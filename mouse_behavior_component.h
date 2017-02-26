#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <vector>
#include "component.h"
#include "mouse_sprite_state.h"

class MouseBehaviorComponent : public Component {
	SDL_Rect* camera;
	float cameraHorizontalFloat;
	float cameraVerticalFloat;
	bool isMovable = false;

	float prevX;
	float prevY;
	int prevState;

	float defaultVx;
	float defaultVy;

	float trackingNumber;

	
public:
	bool resetStateIndicator;

	void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera);
	virtual void Init();
	void Update(float dt);
	void Receive(int message);
	void Destroy();
	void Move(float distanceX, float distanceY);

	void ChangeSpeedX(float newVx) {
		gameEntity->vx = newVx;
	}
	void ChangeSpeedY(float newVy) {
		gameEntity->vy = newVy;
	}

	bool isOnTheGround(int currentState) {
		switch (currentState) {
		case MouseSpriteState::STATE_STAND:
		case MouseSpriteState::STATE_STAND_RIGHT:
		case MouseSpriteState::STATE_WALK:
		case MouseSpriteState::STATE_WALK_RIGHT:
			return true;
		}
		return false;
	}

	int canJumpBackRight = -1;
	int canJumpBackLeft = -1;

	int goingToJumpTo = 0;
	bool jumpAgainstWall = false;

	const static int BOUNCE_SPEED_Y = 430;

private:
	void MoveCamera();

	void ProcessInput(AvancezLib::KeyStatus keys, float dt);

};