#pragma once
#include "game_entity.h"
#include "big_cat.h"
#include "tile_sprite_state.h"
#include "object_pool.h"
#include "big_cat_behavior_component.h"
#include "big_cat_collision_rule.h"
#include "level.h"
#include "rainbow_controller.h"

class BigCatController : public GameEntity {
	AvancezLib* system;
	SDL_Rect* camera;
	//std::vector<Cat*> cats;
	ObjectPool<BigCat> bigCatPool;
	Level* level;
	Mouse* mouse;
	BigCatSpriteState* spriteState;
	RainbowController* rainbowController;

	float spawnDelayTime = 0.f;
	float spawnInterval = .5f;
	bool canSpawn = true;
	bool firstTimeSpawn = true;
	int maximumSpawn = 1;
	int activeCats = 0;
public:
	void Create(AvancezLib* system, SDL_Rect* camera, Level* level, Mouse* mouse, RainbowController* rainbowController) {
		GameEntity::Create();
		this->system = system;
		this->camera = camera;
		this->level = level;
		this->mouse = mouse;
		this->rainbowController = rainbowController;

		spriteState = new BigCatSpriteState();
		spriteState->Create(system);

		//setup object pool
		bigCatPool.Create(2);

		for (auto it = bigCatPool.pool.begin(); it != bigCatPool.pool.end(); it++) {
			BigCat* bigCat = (*it);

			SpriteSheetRenderComponent* spriteComponent = new SpriteSheetRenderComponent();
			spriteComponent->Create(system, bigCat, nullptr, spriteState, true, camera, true);
			BigCatBehaviorComponent* behaviorComponent = new BigCatBehaviorComponent();
			behaviorComponent->Create(system, bigCat, nullptr, camera, level->getItemPool());
			BigCatCollisionRule* collisionRule = new BigCatCollisionRule();
			collisionRule->Create(bigCat, camera, behaviorComponent);
			MapCollideComponent* mapCollideComponent = new MapCollideComponent();
			mapCollideComponent->Create(system, bigCat, nullptr, level->getTileMapPool());
			CollidePoolComponent* ropeCollideComponent = new CollidePoolComponent();
			ropeCollideComponent->Create(system, bigCat, nullptr, (ObjectPool<GameEntity>*)level->getRopePool());
			CollidePoolComponent* doorCollideCompponent = new CollidePoolComponent();
			doorCollideCompponent->Create(system, bigCat, nullptr, (ObjectPool<GameEntity>*)level->getDoorPool());
			CollidePoolComponent* rainbowCollideComponent = new CollidePoolComponent();
			rainbowCollideComponent->Create(system, bigCat, nullptr, (ObjectPool<GameEntity>*)rainbowController->getRainbowPool());
			CollidePoolComponent* itemCollideComponent = new CollidePoolComponent();
			itemCollideComponent->Create(system, bigCat, nullptr, (ObjectPool<GameEntity>*)level->getItemPool());

			bigCat->Create();
			bigCat->SetCollisionRule(collisionRule);
			bigCat->AddComponent(spriteComponent);
			bigCat->AddComponent(mapCollideComponent);
			bigCat->AddComponent(ropeCollideComponent);
			bigCat->AddComponent(doorCollideCompponent);
			bigCat->AddComponent(rainbowCollideComponent);
			bigCat->AddComponent(itemCollideComponent);
			bigCat->AddBehaviorComponent(behaviorComponent);

			bigCat->AddReceiver(this);
			this->AddReceiver(bigCat);

		}

	}

	ObjectPool<BigCat>* getCats() {
		return &bigCatPool;
	}

	void Init() {
		GameEntity::Init();
		firstTimeSpawn = true;
		SDL_Log("BigCatController::Init");
	}


	void Update(float dt) {
		if (!enabled) return;
		for (auto go = bigCatPool.pool.begin(); go != bigCatPool.pool.end(); go++) {
			(*go)->Update(dt);
		}

		if (!canSpawn) {
			spawnDelayTime += dt;
			if (spawnDelayTime > spawnInterval) {
				spawnDelayTime = 0;
				canSpawn = true;
			}
		}

		if (firstTimeSpawn) {
			if (canSpawn) {
				switch (activeCats) {
				case 0:
					Spawn(24 * TileSpriteState::TILE_WIDTH, 2 * TileSpriteState::TILE_HEIGHT);
					break;
				case 1:
					Spawn(12 * TileSpriteState::TILE_WIDTH, 10 * TileSpriteState::TILE_HEIGHT);
					break;
				case 2:
					Spawn();
					break;
				case 3:
					Spawn();
					break;
				default:
					Spawn();
					break;
				}
			}
		}
		else {
			if (canSpawn) {
				if (activeCats < maximumSpawn) {
					Spawn();
				}
			}
		}

		if (activeCats == maximumSpawn) {
			firstTimeSpawn = false;
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

		if (m->getMessageType() == POSSESS_ITEM) {
			Send(m);
		}

		if (m->getMessageType() == UPDATE_SCORE) {
			Send(new Message(m->getMessageType(), m->getArg1(), 1000));
		}

		if (m->getMessageType() == MOUSE_DIE) {
			Hide();
		}

		if (m->getMessageType() == LEVEL_CLEAR) {
			Hide();
		}
	}

	void Hide() {
		for (auto go = bigCatPool.pool.begin(); go != bigCatPool.pool.end(); go++) {
			(*go)->enabled = false;
		}
	}

	void RoundInit() {
		Hide();
		activeCats = 0;
	}

	void Spawn() {
		Spawn(13 * TileSpriteState::TILE_WIDTH, 1 * TileSpriteState::TILE_HEIGHT);
	}
	void Spawn(int x, int y) {
		MakeCat(x, y);
		activeCats++;
		canSpawn = false;
	}

	void MakeCat(int x, int y) {
		BigCat* bigCat = bigCatPool.FirstAvailable();
		if (bigCat == NULL) {
			return;
		}

		bigCat->Init(x, y);
	}
};