#pragma once
#include "game_entity.h"
#include "mouse.h"
#include <set>
#include "mouse_behavior_component.h"
#include "mouse_sprite_state.h"
#include "component.h"

class Game : public GameEntity {
public:
	virtual void Create(AvancezLib* system) {
		SDL_Log("Game::Create");
		this->system = system;
		
		// Mouse init
		mouse = new Mouse();
		MouseBehaviorComponent* mouseBehaviorComponent = new MouseBehaviorComponent();
		mouseBehaviorComponent->Create(system, mouse, &gameEntities);
		SpriteSheetRenderComponent* mouseSpriteSheetRenderComponent = new SpriteSheetRenderComponent();
		MouseSpriteState* mouseSpriteState = new MouseSpriteState();
		mouseSpriteState->Create(system);
		mouseSpriteSheetRenderComponent->Create(system, mouse, &gameEntities, mouseSpriteState);
		mouse->Create();
		mouse->AddComponent(mouseBehaviorComponent);
		mouse->AddComponent(mouseSpriteSheetRenderComponent);
		mouse->AddReceiver(this);
		gameEntities.insert(mouse);
	}

	virtual void Init()
	{
		mouse->Init();

		enabled = true;
		game_over = false;
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
			game_over = true;

		if (m == ALIEN_HIT)
			score += POINTS_PER_ALIEN * game_speed;
	}

	virtual void Update(float dt)
	{
		//if (IsGameOver())
		//	dt = 0.f;

		for (auto go = gameEntities.begin(); go != gameEntities.end(); go++)
			(*go)->Update(dt);

		// check if there are still active aliens
		// if not, send a message to re-init the level
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
	Mouse* mouse;
	AvancezLib* system;
	bool game_over;
	unsigned int score = 0;
	std::set<GameEntity*> gameEntities;
};