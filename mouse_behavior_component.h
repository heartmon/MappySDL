#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <vector>
#include "component.h"
#include "mouse_sprite_state.h"

class Mouse;
class MouseBehaviorComponent : public Component {
	SDL_Rect* camera;
	int life = 2;
	Mouse* gameEntity;

	float cameraHorizontalFloat;
	float cameraVerticalFloat;
	bool isMovable = false;

	float prevX;
	float prevY;
	int prevState;

	float defaultVx;
	float defaultVy;

	float trackingNumber;

	float spaceTriggerInterval = 0.35f;
	float spaceTriggerTime = 0;
	bool canSpace = true;

	float knockbackInterval = 0.2f;
	float knockbackTime = 0;
	int knockbackDirection = 0;
	float knockbackSpeed = 400.f;

	float deadInterval = .7f;
	float deadTime = 0;
	float deadMovingInterval = 1.5f;
	float deadMovingTime = 0;
	
public:
	bool resetStateIndicator;

	void Create(AvancezLib* system, Mouse * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera);
	virtual void Init();
	virtual void RoundInit();
	void Update(float dt);
	void Receive(int message);
	void Destroy();
	void Move(float distanceX, float distanceY);

	void ChangeSpeedX(float newVx);
	void ChangeSpeedY(float newVy);

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

	void WhenDoorOpen(GameEntity* door);
	void WhenDoorClose(GameEntity* door);

	int canJumpBackRight = -1;
	int canJumpBackLeft = -1;

	int goingToJumpTo = 0;
	bool jumpAgainstWall = false;

	const static int BOUNCE_SPEED_Y = 430;

	void WhenHeadHit();
	void WhenDie();

	bool isGoingToDie = false;

private:
	bool toBeKnockedBack = false;

};