#pragma once
#include "game_entity.h"
#include "cat.h"
#include "tile_sprite_state.h"
#include "object_pool.h"
#include "cat_behavior_component.h"
#include "cat_collision_rule.h"
#include "level.h"
#include "rainbow_controller.h"

class CatController : public GameEntity {
	AvancezLib* system;
	SDL_Rect* camera;
	//std::vector<Cat*> cats;
	ObjectPool<Cat> cats_pool;
	Level* level;
	Mouse* mouse;
	CatSpriteState* spriteState;
	RainbowController* rainbowController;

	float spawnDelayTime = 0.f;
	float spawnInterval = .3f;
	bool canSpawn = true;
	bool roundSpawn = true;
	int maximumSpawn = 4;
	int activeCats = 0;
	int levelNo;
public:
	void Create(AvancezLib* system, SDL_Rect* camera, Level* level, Mouse* mouse, RainbowController* rainbowController) {
		GameEntity::Create();
		this->system = system;
		this->camera = camera;
		this->level = level;
		this->mouse = mouse;
		this->rainbowController = rainbowController;

		spriteState = new CatSpriteState();
		spriteState->Create(system);

		//setup object pool
		cats_pool.Create(7);

		for (auto it = cats_pool.pool.begin(); it != cats_pool.pool.end(); it++) {
			Cat* cat = (*it);

			SpriteSheetRenderComponent* spriteComponent = new SpriteSheetRenderComponent();
			spriteComponent->Create(system, cat, nullptr, spriteState, true, camera, true);
			CatBehaviorComponent* behaviorComponent = new CatBehaviorComponent();
			behaviorComponent->Create(system, cat, nullptr, camera, mouse);
			CatCollisionRule* collisionRule = new CatCollisionRule();
			collisionRule->Create(cat, camera, behaviorComponent);
			MapCollideComponent* mapCollideComponent = new MapCollideComponent();
			mapCollideComponent->Create(system, cat, nullptr, level->getTileMapPool());
			CollidePoolComponent* ropeCollideComponent = new CollidePoolComponent();
			ropeCollideComponent->Create(system, cat, nullptr, (ObjectPool<GameEntity>*)level->getRopePool());
			CollidePoolComponent* doorCollideCompponent = new CollidePoolComponent();
			doorCollideCompponent->Create(system, cat, nullptr, (ObjectPool<GameEntity>*)level->getDoorPool());
			CollidePoolComponent* rainbowCollideComponent = new CollidePoolComponent();
			rainbowCollideComponent->Create(system, cat, nullptr, (ObjectPool<GameEntity>*)rainbowController->getRainbowPool());

			cat->Create();
			cat->SetCollisionRule(collisionRule);
			cat->AddComponent(spriteComponent);
			cat->AddComponent(mapCollideComponent);
			cat->AddComponent(ropeCollideComponent);
			cat->AddComponent(doorCollideCompponent);
			cat->AddComponent(rainbowCollideComponent);
			cat->AddBehaviorComponent(behaviorComponent);

			cat->AddReceiver(this);
			this->AddReceiver(cat);

		}

	}

	ObjectPool<Cat>* getCats() {
		return &cats_pool;
	}

	void Init(int levelNo) {
		GameEntity::Init();
		roundSpawn = true;
		SDL_Log("CatController::Init");
		this->levelNo = levelNo;

		if (levelNo <= 2) {
			maximumSpawn = 1;
		}
		else if (levelNo <= 4) {
			maximumSpawn = 4;
		}
		else if (levelNo <= 6) {
			maximumSpawn = 5;
		}
		else {
			maximumSpawn = 6;
		}
	}


	void Update(float dt) {
		for (auto go = cats_pool.pool.begin(); go != cats_pool.pool.end(); go++) {
			(*go)->Update(dt);
		}

		if (!canSpawn) {
			spawnDelayTime += dt;
			if (spawnDelayTime > spawnInterval) {
				spawnDelayTime = 0;
				canSpawn = true;
			}
		}

		if (activeCats < maximumSpawn) {
			if (roundSpawn) {
				switch (activeCats) {
				case 0:
					Spawn(24 * TileSpriteState::TILE_WIDTH, 3 * TileSpriteState::TILE_HEIGHT);
					break;
				case 1:
					Spawn(13 * TileSpriteState::TILE_WIDTH, 9 * TileSpriteState::TILE_HEIGHT);
					break;
				case 2:
					Spawn(13 * TileSpriteState::TILE_WIDTH, 2 * TileSpriteState::TILE_HEIGHT);
					break;
				case 3:
					Spawn(13 * TileSpriteState::TILE_WIDTH, 2 * TileSpriteState::TILE_HEIGHT);
					break;
				default:
					Spawn();
					break;
				}
			}
			else {
				if (canSpawn) {
					Spawn();
				}
			}
		}

		if (activeCats == maximumSpawn) {
			roundSpawn = false;
		}

		

	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == TOGGLE_DOOR) {
			Send(m);
		}

		if (m->getMessageType() == DOOR_OPEN || m->getMessageType() == DOOR_CLOSE) {
			Send(m);
		}

		if (m->getMessageType() == CAT_DIE) {
			activeCats--;
		}

		if (m->getMessageType() == MOUSE_DIE) {
			Hide();
		}

		if (m->getMessageType() == LEVEL_CLEAR) {
			Hide();
		}
	}

	void Hide() {
		for (auto go = cats_pool.pool.begin(); go != cats_pool.pool.end(); go++) {
			(*go)->enabled = false;
		}
	}

	void RoundInit() {
		Hide();
		activeCats = 0;
		roundSpawn = true;
	}

	void Spawn() {
		Spawn(14.5 * TileSpriteState::TILE_WIDTH, 1 * TileSpriteState::TILE_HEIGHT);
	}
	void Spawn(float x, float y) {
		MakeCat(x, y);
		activeCats++;
		canSpawn = false;
	}

	void MakeCat(int x, int y) {
		Cat* cat = cats_pool.FirstAvailable();
		if (cat == NULL) {
			return; 
		}

		cat->Init(x, y);
	}
};