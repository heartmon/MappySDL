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

	std::vector<Rope*>* ropeArray;
	std::vector<Door*>* doorArray;
	std::vector<Item*>* itemArray;

	int levelNo = 1;

	const int START_CAMERA_X = LEVEL_WIDTH - SCREEN_WIDTH;
	const int START_MOUSE_X = 280;
	const int START_MOUSE_Y = 400;

public:
	virtual void Create(AvancezLib* system) {
		SDL_Log("Game::Create");
		this->system = system;

		camera = new SDL_Rect();

		// Tile level init
		level = new Level();
		level->Create(system, camera);
		gameEntities.push_back(level);

		// Item Arrays from Level
		ropeArray = level->getRopeArray();
		doorArray = level->getDoorArray();
		itemArray = level->getItemArray();

		//Door Toggle
		doorToggle = new DoorToggle();
		doorToggle->Create(doorArray, camera);
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
		mapCollideComponent->Create(system, mouse, &gameEntities, level->getTileMap());

		CollideComponent* ropeCollideComponent = new CollideComponent();
		ropeCollideComponent->Create(system, mouse, &gameEntities, (std::vector<GameEntity*>*)ropeArray);

		CollideComponent* doorCollideCompponent = new CollideComponent();
		doorCollideCompponent->Create(system, mouse, &gameEntities, (std::vector<GameEntity*>*)doorArray);

		CollideComponent* itemCollideComponent = new CollideComponent();
		itemCollideComponent->Create(system, mouse, &gameEntities, (std::vector<GameEntity*>*)itemArray);

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
		SDL_Log(" --- Start new round --- ");
		PositionRoundInit();
		EntityRoundInit();
	}

	void PositionRoundInit() {
		camera->x = START_CAMERA_X;
		camera->y = 0;
		camera->w = SCREEN_WIDTH;
		camera->h = SCREEN_HEIGHT;

		mouse->horizontalPosition = START_MOUSE_X;
		mouse->verticalPosition = START_MOUSE_Y;
	}

	void ReceiverInit() {
		mouse->AddReceiver(this);
		mouse->AddReceiver(doorToggle);
		mouse->AddReceiver(info);

		this->AddReceiver(info);

		scoreController->AddReceiver(info);
		catController->AddReceiver(doorToggle);

		for (std::vector<Rope*>::iterator it = ropeArray->begin(); it != ropeArray->end(); ++it) {
			Rope* rope = *it;
			mouse->AddReceiver(rope);
		}

		for (std::vector<Door*>::iterator it = doorArray->begin(); it != doorArray->end(); ++it) {
			Door* door = *it;
			mouse->AddReceiver(door);
			door->AddReceiver(mouse);
			door->AddReceiver(rainbowController);
			door->AddReceiver(catController);
		}

		for (std::vector<Item*>::iterator it = itemArray->begin(); it != itemArray->end(); ++it) {
			Item* item = *it;
			item->AddReceiver(scoreController);
		}
	}

	void EntityRoundInit() {
		mouse->RoundInit();
		for (std::vector<Rope*>::iterator it = ropeArray->begin(); it != ropeArray->end(); ++it) {
			Rope* rope = *it;
			rope->RoundInit();
		}

		//catController->RoundInit();

		this->Send(new Message(UPDATE_LEVEL, this, levelNo));
	}

	virtual void Init()
	{
		PositionRoundInit();

		info->Init();
		mouse->Init();
		level->Init();
		doorToggle->Init();
		scoreController->Init();
		rainbowController->Init();
		catController->Init();
		catController->Spawn();

		ReceiverInit();
		EntityRoundInit();

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