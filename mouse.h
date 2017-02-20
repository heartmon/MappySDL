#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "mouse_sprite_state.h"

class Mouse : public GameEntity {
	//Constants


	//Properties
	//AvancezLib* system;
	//Sprite* sprite; // will use cliping
	//int currentState;

public:
	////State of mouse
	////enum State { Saturday, Sunday, Monday, Tuesday, Wednesday, Thursday, Friday };
	//const static int STATE_STAND = 0;
	//const static int STATE_WALK = 1;
	//const static int STATE_PREJUMP = 2;
	//const static int STATE_JUMP = 3;
	//const static int STATE_KNOCKBACK = 4;
	//const static int STATE_DEAD = 5;


	virtual ~Mouse() { SDL_Log("Mouse::~Mouse"); }
	virtual void Init()
	{
		SDL_Log("Mouse::Init");
		GameEntity::Init();

		horizontalPosition = 30;
		verticalPosition = 0;
		setCurrentStateType(MouseSpriteState::STATE_STAND);
	}
	virtual void Receive(Message m)
	{
		if (m == HIT)
		{
			SDL_Log("Mouse::Hit!");
		}
	}

private:
	//Collision box
	//SDL_Rect collider;
};