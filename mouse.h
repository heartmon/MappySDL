#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "mouse_sprite_state.h"

class Mouse : public GameEntity {
public:
	int lives;

	//
	virtual void Create(float x, float y) {
		GameEntity::Create();
		horizontalPosition = x;
		verticalPosition = y;
	}
	virtual void Init()
	{
		SDL_Log("Mouse::Init");
		lives = 2;
		GameEntity::Init();

		setCurrentStateType(MouseSpriteState::STATE_STAND);

		size->w = MouseSpriteState::SPRITE_WIDTH;
		size->h = MouseSpriteState::SPRITE_HEIGHT;

		direction = GameEntity::LEFT;

		vx = 160.0f;
	}

	virtual void Receive(Message* m);
	virtual void RoundInit();

	virtual std::string getName() {
		return CLASS_MOUSE;
	}

	virtual SDL_Rect* getSize() {
		return size;
	}

private:

};