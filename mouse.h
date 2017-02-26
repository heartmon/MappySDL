#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "mouse_sprite_state.h"

class Mouse : public GameEntity {
public:
	virtual void Create() {
		GameEntity::Create();
	}
	virtual ~Mouse() { SDL_Log("Mouse::~Mouse"); }
	virtual void Init()
	{
		SDL_Log("Mouse::Init");
		GameEntity::Init();

		horizontalPosition = 240;
		verticalPosition = 0;
		setCurrentStateType(MouseSpriteState::STATE_STAND);

		size->w = MouseSpriteState::SPRITE_WIDTH;
		size->h = MouseSpriteState::SPRITE_HEIGHT;

		w = MouseSpriteState::SPRITE_WIDTH;
		h = MouseSpriteState::SPRITE_HEIGHT;

		vx = 160.0f;
	}
	//virtual void Receive(Message* m)
	//{
	//	//SDL_Log("Mouse::Hit!");
	//}

	virtual std::string getName() {
		return CLASS_MOUSE;
	}

	virtual SDL_Rect* getSize() {
		return size;
	}

private:

};