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

class Game : public GameEntity {
	Mouse* mouse;
	AvancezLib* system;
	bool game_over;
	unsigned int score = 0;
	std::vector<GameEntity*> gameEntities;
	float lastTime;
	Level* level;
	SDL_Rect* camera;
public:
	virtual void Create(AvancezLib* system) {
		SDL_Log("Game::Create");
		this->system = system;

		camera = new SDL_Rect();
		camera->x = 0;
		camera->y = 0;
		camera->w = SCREEN_WIDTH;
		camera->h = SCREEN_HEIGHT;

		// Tile level init
		level = new Level();
		level->Create(system, camera);
		gameEntities.push_back(level);

		// Mouse init
		mouse = new Mouse();
		MouseCollisionRule* mouseCollisionRule = new MouseCollisionRule();
		mouseCollisionRule->Create(mouse);
		MouseBehaviorComponent* mouseBehaviorComponent = new MouseBehaviorComponent();
		mouseBehaviorComponent->Create(system, mouse, &gameEntities, camera);
		SpriteSheetRenderComponent* mouseSpriteSheetRenderComponent = new SpriteSheetRenderComponent();
		MouseSpriteState* mouseSpriteState = new MouseSpriteState();
		mouseSpriteState->Create(system);
		mouseSpriteSheetRenderComponent->Create(system, mouse, &gameEntities, mouseSpriteState);
		MapCollideComponent* mapCollideComponent = new MapCollideComponent();
		mapCollideComponent->Create(system, mouse, &gameEntities, level->getTileMap());
		mouse->Create();
		mouse->AddComponent(mouseBehaviorComponent);
		mouse->AddComponent(mouseSpriteSheetRenderComponent);
		mouse->AddComponent(mapCollideComponent);
		mouse->AddReceiver(this);
		mouse->SetCollisionRule(mouseCollisionRule);
		gameEntities.push_back(mouse);

	}

	virtual void Init()
	{
		mouse->Init();
		level->Init();

		enabled = true;
		game_over = false;
	}

	virtual void Receive(Message* m)
	{
		if (m->getMessageType() == GAME_OVER)
			game_over = true;

		if (m->getMessageType() == ALIEN_HIT)
			score += POINTS_PER_ALIEN * game_speed;
	}

	virtual void Update(float dt)
	{
		//if (IsGameOver())
		//	dt = 0.f;
		float newTime = system->getElapsedTime();

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Update(dt);

		float an = system->getElapsedTime();

		//SDL_Log("Game loop update time %f", an - newTime);
		// check if there are still active aliens
		// if no, send a message to re-init the level
		//bool are_aliens_still_there = false;
		//for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		//	are_aliens_still_there |= (*alien)->enabled;
		//if (!are_aliens_still_there)
		//{
		//	// level win!
		//	game_speed += 0.4f;
		//	aliens_grid->Init();
		//}
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Destroy();

		delete mouse;
	}

private:

};