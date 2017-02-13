#pragma once
#include "game_entity.h"
#include "avancezlib.h"

class MouseRenderComponent;
class MouseInputComponent;
class Mouse : public GameEntity {
	//Constants


	//Properties
	AvancezLib* system;
	Sprite* sprite; // will use cliping
	int currentState;

public:
	//State of mouse
	//enum State { Saturday, Sunday, Monday, Tuesday, Wednesday, Thursday, Friday };
	const static int STATE_STAND = 0;
	const static int STATE_WALK = 1;
	const static int STATE_JUMP = 2;
	const static int STATE_KNOCKBACK = 3;
	const static int STATE_DEAD = 4;

	Mouse();
	void Init(AvancezLib* system, MouseRenderComponent* renderComponent, MouseInputComponent* inputComponent);
	void Draw();
	void Update(float dt);
	void Destroy();
	int getCurrentState();
	void setCurrentState(int state);
	int direction;

private:
	MouseRenderComponent* renderComponent;
	MouseInputComponent* inputComponent;
};