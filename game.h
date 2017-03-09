#pragma once
#include "game_entity.h"
#include "mouse.h"
#include <vector>
#include "mouse_behavior_component.h"
#include "mouse_sprite_state.h"
#include "component.h"
#include "level.h"
#include "global_constant.h"
#include "mouse_collision_rule.h"
#include "door_toggle.h"
#include "rainbow_controller.h"
#include "info.h"
#include "item.h"
#include "score_controller.h"
#include "cat_controller.h"

class Game : public GameEntity {
	Mouse* mouse;
	AvancezLib* system;
	bool game_over;
	unsigned int score = 0;
	std::vector<GameEntity*> gameEntities;
	float lastTime;
	Level* level;
	SDL_Rect* camera;
	DoorToggle* doorToggle;
	RainbowController* rainbowController;
	Info* info;
	ScoreController* scoreController;
	CatController* catController;

	ObjectPool<Item>* itemPool;
	ObjectPool<Rope>* ropePool;
	ObjectPool<Tile>* tileMapPool;
	ObjectPool<Door>* doorPool;

	int levelNo = 1;

	const int START_CAMERA_X = LEVEL_WIDTH - SCREEN_WIDTH;
	const int START_MOUSE_X = 280;
	const int START_MOUSE_Y = 400;

	bool doonce = false;

public:
	virtual void Create(AvancezLib* system) {
		SDL_Log(" ---=== CREATE GAME.H ===--- ");
		this->system = system;

		camera = new SDL_Rect();

		// Tile level init
		level = new Level();
		level->Create(system, camera);
		gameEntities.push_back(level);

		// Item Arrays from Level
		ropePool = level->getRopePool();
		doorPool = level->getDoorPool();
		itemPool = level->getItemPool();
		tileMapPool = level->getTileMapPool();

		//Door Toggle
		doorToggle = new DoorToggle();
		doorToggle->Create(doorPool, camera);
		gameEntities.push_back(doorToggle);


		// Mouse init
		mouse = new Mouse();


		//Cat controller
		catController = new CatController();
		catController->Create(system, camera, level, mouse);
		gameEntities.push_back(catController);

		MouseBehaviorComponent* mouseBehaviorComponent = new MouseBehaviorComponent();
		mouseBehaviorComponent->Create(system, mouse, &gameEntities, camera);
		MouseCollisionRule* mouseCollisionRule = new MouseCollisionRule();
		mouseCollisionRule->Create(mouse, camera, mouseBehaviorComponent);
		SpriteSheetRenderComponent* mouseSpriteSheetRenderComponent = new SpriteSheetRenderComponent();
		MouseSpriteState* mouseSpriteState = new MouseSpriteState();
		mouseSpriteState->Create(system);
		mouseSpriteSheetRenderComponent->Create(system, mouse, &gameEntities, mouseSpriteState);
		MapCollideComponent* mapCollideComponent = new MapCollideComponent();
		mapCollideComponent->Create(system, mouse, &gameEntities, tileMapPool);

		CollidePoolComponent* ropeCollideComponent = new CollidePoolComponent();
		ropeCollideComponent->Create(system, mouse, &gameEntities, (ObjectPool<GameEntity>*)ropePool);

		CollidePoolComponent* doorCollideCompponent = new CollidePoolComponent();
		doorCollideCompponent->Create(system, mouse, &gameEntities, (ObjectPool<GameEntity>*)doorPool);

		CollidePoolComponent* itemCollideComponent = new CollidePoolComponent();
		itemCollideComponent->Create(system, mouse, &gameEntities, (ObjectPool<GameEntity>*)itemPool);

		CollidePoolComponent* catsCollideComponent = new CollidePoolComponent();
		catsCollideComponent->Create(system, mouse, &gameEntities, (ObjectPool<GameEntity>*)catController->getCats());

		mouse->Create(START_MOUSE_X, START_MOUSE_Y);
		mouse->AddBehaviorComponent(mouseBehaviorComponent);
		mouse->AddComponent(mouseSpriteSheetRenderComponent);
		mouse->AddComponent(mapCollideComponent);
		mouse->AddComponent(itemCollideComponent);
		mouse->AddComponent(ropeCollideComponent);
		mouse->AddComponent(doorCollideCompponent);
		mouse->AddComponent(catsCollideComponent);
		
		mouse->SetCollisionRule(mouseCollisionRule);
		gameEntities.push_back(mouse);


		//Rainbow controller
		rainbowController = new RainbowController();
		rainbowController->Create(system, camera);
		gameEntities.push_back(rainbowController);

		// Info
		info = new Info();
		info->Create(system);
		gameEntities.push_back(info);
		

		//Score controller
		scoreController = new ScoreController();
		scoreController->Create(system, camera);
		gameEntities.push_back(scoreController);


	}

	void RoundInit() {
		SDL_Log(" --- YOU HAVE LIFE LEFT ==> CONTINUE WITH THE SAME LEVEL --- ");
		SetStartPosition();
		CleanUpEachRound();
	}

	void SetStartPosition() {
		camera->x = START_CAMERA_X;
		camera->y = 0;
		camera->w = SCREEN_WIDTH;
		camera->h = SCREEN_HEIGHT;

		mouse->horizontalPosition = START_MOUSE_X;
		mouse->verticalPosition = START_MOUSE_Y;
	}

	void SetupObserver() {
		SDL_Log("============= Observer / Observable Setup =================");

		this->AddReceiver(info);

		scoreController->AddReceiver(info);
		catController->AddReceiver(doorToggle);

		mouse->AddReceiver(this);
		mouse->AddReceiver(doorToggle);
		mouse->AddReceiver(info);

		for (auto it = ropePool->pool.begin(); it != ropePool->pool.end(); ++it) {
			if (!(*it)->enabled) continue;
			Rope* rope = *it;
			mouse->AddReceiver(rope);
		}

		for (auto it = doorPool->pool.begin(); it != doorPool->pool.end(); ++it) {
			if (!(*it)->enabled) continue;
			Door* door = *it;
			mouse->AddReceiver(door);
			door->AddReceiver(mouse);
			door->AddReceiver(rainbowController);
			door->AddReceiver(catController);
		}

		for (auto it = itemPool->pool.begin(); it != itemPool->pool.end(); ++it) {
			if (!(*it)->enabled) continue;
			Item* item = *it;
			item->AddReceiver(scoreController);
		}
		
	}

	void CleanUpEachRound() {
		mouse->RoundInit();
		for (auto it = ropePool->pool.begin(); it != ropePool->pool.end(); ++it) {
			if (!(*it)->enabled) continue;
			Rope* rope = *it;
			rope->RoundInit();
		}

		catController->RoundInit();
		//catController->Spawn();

		this->Send(new Message(UPDATE_LEVEL, this, levelNo));
	}

	virtual void Init(int levelNo)
	{
		this->levelNo = levelNo;

		// Log level start
		SDL_Log(" ---=== INIT GAME SCREEN LEVEL %d ===--- ", levelNo);

		// Set initial position
		SetStartPosition();

		// Init game entities
		info->Init();
		mouse->Init();
		doorToggle->Init();
		scoreController->Init();
		rainbowController->Init();
		catController->Init();

		// Init level
		level->Init(levelNo);

		// Init once
		if (!doonce) {
			SetupObserver();
			doonce = true;
		}

		// clean up for round
		CleanUpEachRound();

		// finishing init
		enabled = true;
		game_over = false;
	}


	virtual void Receive(Message* m)
	{
		if (m->getMessageType() == LIFE_DECREASE) {
			RoundInit();
		}

		if (m->getMessageType() == GAME_OVER)
			game_over = true;

		if (m->getMessageType() == LEVEL_CLEAR) {
			levelNo++;
		}
	}

	virtual void Update(float dt)
	{
		//if (IsGameOver())
		//	dt = 0.f;

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Update(dt);
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Destroy();

		delete mouse;
	}

	bool isGameOver() {
		return game_over;
	}

private:

};