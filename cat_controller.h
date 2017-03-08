#pragma once
#include "game_entity.h"
#include "cat.h"
#include "tile_sprite_state.h"
#include "object_pool.h"
#include "cat_behavior_component.h"
#include "cat_collision_rule.h"
#include "level.h"

class CatController : public GameEntity {
	AvancezLib* system;
	SDL_Rect* camera;
	//std::vector<Cat*> cats;
	ObjectPool<Cat> cats_pool;
	Level* level;
	Mouse* mouse;
	CatSpriteState* spriteState;
public:
	void Create(AvancezLib* system, SDL_Rect* camera, Level* level, Mouse* mouse) {
		GameEntity::Create();
		this->system = system;
		this->camera = camera;
		this->level = level;
		this->mouse = mouse;
		cats_pool.Create(20);

		spriteState = new CatSpriteState();
		spriteState->Create(system);
	}

	ObjectPool<Cat>* getCats() {
		return &cats_pool;
	}

	void Update(float dt) {
		for (auto go = cats_pool.pool.begin(); go != cats_pool.pool.end(); go++) {
			(*go)->Update(dt);
		}
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == TOGGLE_DOOR) {
			Send(m);
		}

		if (m->getMessageType() == DOOR_OPEN || m->getMessageType() == DOOR_CLOSE) {
			//SDL_Log("CAT CONTROLLER GET DOOR OPEN =====================");
			Send(m);
		}
	}

	void RoundInit() {
		//cats_pool.Deallocate();
		//cats_pool.Create(20);
	
		//for (auto go = cats_pool.pool.begin(); go != cats_pool.pool.end(); go++) {
		//	(*go)->enabled = false;
		//}
		Spawn();
	}

	void Spawn() {
		Spawn(21 * TileSpriteState::TILE_WIDTH, 1 * TileSpriteState::TILE_HEIGHT);
	}
	void Spawn(int x, int y) {
		MakeCat(x, y);
	}

	void MakeCat(int x, int y) {
		Cat* cat = cats_pool.FirstAvailable();
		if (cat == NULL) {
			return; 
		}

		cat->Create(x, y);
		
		

		SpriteSheetRenderComponent* spriteComponent = new SpriteSheetRenderComponent();
		spriteComponent->Create(system, cat, nullptr, spriteState, true, camera, true);
		CatBehaviorComponent* behaviorComponent = new CatBehaviorComponent();
		behaviorComponent->Create(system, cat, nullptr, camera, mouse);
		CatCollisionRule* collisionRule = new CatCollisionRule();
		collisionRule->Create(cat, camera, behaviorComponent);
		MapCollideComponent* mapCollideComponent = new MapCollideComponent();
		mapCollideComponent->Create(system, cat, nullptr, level->getTileMap());
		CollideComponent* ropeCollideComponent = new CollideComponent();
		ropeCollideComponent->Create(system, cat, nullptr, (std::vector<GameEntity*>*)level->getRopeArray());
		CollideComponent* doorCollideCompponent = new CollideComponent();
		doorCollideCompponent->Create(system, cat, nullptr, (std::vector<GameEntity*>*)level->getDoorArray());


		cat->SetCollisionRule(collisionRule);
		cat->AddComponent(spriteComponent);
		cat->AddComponent(mapCollideComponent);
		cat->AddComponent(ropeCollideComponent);
		cat->AddComponent(doorCollideCompponent);
		cat->AddBehaviorComponent(behaviorComponent);
		
		cat->AddReceiver(this);
		this->AddReceiver(cat);
		
		cat->Init();
	}
};