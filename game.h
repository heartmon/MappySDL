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
#include "door_controller.h"
#include "rainbow_controller.h"
#include "info.h"
#include "item.h"
#include "score_controller.h"
#include "cat_controller.h"
#include "item_controller.h"
#include "big_cat_controller.h"
#include "sound_controller.h"

class Game : public GameEntity {
	Mouse* mouse;
	AvancezLib* system;
	bool game_over;
	unsigned int score = 0;
	std::vector<GameEntity*> gameEntities;
	float lastTime;
	Level* level;
	SDL_Rect* camera;
	DoorController* doorController;
	RainbowController* rainbowController;
	Info* info;
	ScoreController* scoreController;
	CatController* catController;
	ItemController* itemController;
	BigCatController* bigCatController;
	SoundController* soundController;

	ObjectPool<Item>* itemPool;
	ObjectPool<Rope>* ropePool;
	ObjectPool<Tile>* tileMapPool;
	ObjectPool<Door>* doorPool;
	ObjectPool<Roof>* roofPool;

	int levelNo = 1;

	const int START_CAMERA_X = LEVEL_WIDTH - SCREEN_WIDTH;
	const int START_MOUSE_X = 440;
	const int START_MOUSE_Y = 440;

	bool doonce = false;

	bool isLevelReadyToBeCleared = false;
	float levelClearDelayTimeCount = 0;
	float levelClearDelayInterval = 2.f;

	int highscore = 0;
	int currentscore = 0;

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
		roofPool = level->getRoofPool();

		//Door Toggle
		doorController = new DoorController();
		doorController->Create(doorPool, camera);
		gameEntities.push_back(doorController);


		//Rainbow controller
		rainbowController = new RainbowController();
		rainbowController->Create(system, camera);
		gameEntities.push_back(rainbowController);

		// Mouse init
		mouse = new Mouse();

		// BigCat Controller
		bigCatController = new BigCatController();
		bigCatController->Create(system, camera, level, mouse, rainbowController);
		gameEntities.push_back(bigCatController);


		//Cat controller
		catController = new CatController();
		catController->Create(system, camera, level, mouse, rainbowController);
		gameEntities.push_back(catController);

		MouseBehaviorComponent* mouseBehaviorComponent = new MouseBehaviorComponent();
		mouseBehaviorComponent->Create(system, mouse, &gameEntities, camera);
		MouseCollisionRule* mouseCollisionRule = new MouseCollisionRule();
		mouseCollisionRule->Create(mouse, camera, mouseBehaviorComponent);
		SpriteSheetRenderComponent* mouseSpriteSheetRenderComponent = new SpriteSheetRenderComponent();
		MouseSpriteState* mouseSpriteState = new MouseSpriteState();
		mouseSpriteState->Create(system);
		mouseSpriteSheetRenderComponent->Create(system, mouse, &gameEntities, mouseSpriteState, false, NULL, true);
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

		CollidePoolComponent* bigCatsCollideComponent = new CollidePoolComponent();
		bigCatsCollideComponent->Create(system, mouse, &gameEntities, (ObjectPool<GameEntity>*)bigCatController->getCats());

		mouse->Create(START_MOUSE_X, START_MOUSE_Y);
		mouse->AddBehaviorComponent(mouseBehaviorComponent);
		mouse->AddComponent(mouseSpriteSheetRenderComponent);
		mouse->AddComponent(mapCollideComponent);
		mouse->AddComponent(itemCollideComponent);
		mouse->AddComponent(ropeCollideComponent);
		mouse->AddComponent(doorCollideCompponent);
		mouse->AddComponent(catsCollideComponent);
		mouse->AddComponent(bigCatsCollideComponent);
		
		mouse->SetCollisionRule(mouseCollisionRule);
		gameEntities.push_back(mouse);



		// Info
		info = new Info();
		info->Create(system);
		gameEntities.push_back(info);

		// Item Controller
		itemController = new ItemController();
		itemController->Create(system, camera, level, mouse);
		gameEntities.push_back(itemController);



		//Score controller
		scoreController = new ScoreController();
		scoreController->Create(system, camera);
		gameEntities.push_back(scoreController);

		// Sound controller
		soundController = new SoundController();
		soundController->Create(system);
		gameEntities.push_back(soundController);

		for (auto it = roofPool->pool.begin(); it != roofPool->pool.end(); ++it) {
			GameEntity* roof = (GameEntity*)(*it);
			gameEntities.push_back(roof);
		}
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
		catController->AddReceiver(doorController);
		bigCatController->AddReceiver(doorController);

		mouse->AddReceiver(this);
		mouse->AddReceiver(doorController);
		mouse->AddReceiver(info);
		mouse->AddReceiver(catController);
		doorController->AddReceiver(mouse);

		for (auto it = ropePool->pool.begin(); it != ropePool->pool.end(); ++it) {
			Rope* rope = *it;
			mouse->AddReceiver(rope);
		}

		for (auto it = doorPool->pool.begin(); it != doorPool->pool.end(); ++it) {
			Door* door = *it;
			mouse->AddReceiver(door);
			door->AddReceiver(mouse);
			door->AddReceiver(rainbowController);
			door->AddReceiver(catController);
			door->AddReceiver(bigCatController);
		}

		for (auto it = itemPool->pool.begin(); it != itemPool->pool.end(); ++it) {
			Item* item = *it;
			item->AddReceiver(itemController);
		}

		itemController->AddReceiver(scoreController);
		itemController->AddReceiver(this);
		rainbowController->AddReceiver(scoreController);
		bigCatController->AddReceiver(info);

		this->AddReceiver(soundController);
		mouse->AddReceiver(soundController);
		itemController->AddReceiver(soundController);
		rainbowController->AddReceiver(soundController);
		itemController->AddReceiver(catController);
		itemController->AddReceiver(bigCatController);
		itemController->AddReceiver(mouse);
		mouse->AddReceiver(scoreController);
		mouse->AddReceiver(bigCatController);
	}

	void CleanUpEachRound() {
		mouse->RoundInit();
		for (auto it = ropePool->pool.begin(); it != ropePool->pool.end(); ++it) {
			if (!(*it)->enabled) continue;
			Rope* rope = *it;
			rope->RoundInit();
		}

		catController->RoundInit();
		bigCatController->RoundInit();
		//catController->Spawn();

		this->Send(new Message(UPDATE_LEVEL, this, levelNo));
	}

	virtual void Init()
	{
		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->isStop = false;

		// Log level start
		SDL_Log(" ---=== INIT GAME SCREEN LEVEL %d ===--- ", levelNo);

		// Set initial position
		SetStartPosition();

		// Init game entities
		if (levelNo == 1) {
			mouse->Init();
			info->Init();
		}

		doorController->Init();
		scoreController->Init();
		rainbowController->Init();
		catController->Init(levelNo);
		bigCatController->Init();
		soundController->Init();

		mouse->RoundInit();

		// Init level
		level->Init(levelNo);

		itemController->Init();

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

		Send(new Message(GAME_INIT));
		Send(new Message(HIGH_SCORE, this, getHighscore()));
	}


	virtual void Receive(Message* m)
	{
		if (m->getMessageType() == LIFE_DECREASE) {
			RoundInit();
			Send(new Message(ROUND_INIT));
		}

		if (m->getMessageType() == GAME_OVER) {
			game_over = true;
			levelNo = 1;
			gameSpeed = 1.f;

			//calculate highscore
			if (info->getScoreTotal() > highscore) {
				highscore = info->getScoreTotal();
			}
			currentscore = info->getScoreTotal();
		}

		if (m->getMessageType() == LEVEL_CLEAR) {
			SDL_Log("Level clear");
			isLevelReadyToBeCleared = true;
		}
	}

	virtual void Update(float dt)
	{
		//if (IsGameOver())
		//	dt = 0.f;

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Update(dt);

		if (isLevelReadyToBeCleared) {
			levelClearDelayTimeCount += dt;
			if (levelClearDelayTimeCount > levelClearDelayInterval) {
				levelClearDelayTimeCount = 0;
				isLevelReadyToBeCleared = false;

				levelNo++;
				gameSpeed += .075f;

				if (levelNo >= 7) {
					Receive(new Message(GAME_OVER));
				}
				else {
					Init();
				}
			}
		}

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

	int getHighscore() {
		return highscore;
	}

	int getCurrentScore() {
		return currentscore;
	}

private:

};