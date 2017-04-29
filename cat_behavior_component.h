#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <vector>
#include "component.h"
#include "cat_sprite_state.h"

enum CatAction { NOTHING, CAT_STAND, CAT_MOVE_LEFT, CAT_MOVE_RIGHT, CAT_JUMP_BACK_LEFT, CAT_JUMP_BACK_RIGHT, CAT_TOGGLE_DOOR };
class Cat;
class Mouse;
class Door;
class CatBehaviorComponent : public Component {
	SDL_Rect* camera;
	Cat* cat;
	Cat* gameEntity;
	Mouse* mouse;

	float catWalkingSpeed = 150.f;

	float defaultVx;
	float defaultVy;

	float spaceTriggerInterval = 0.35f;
	float spaceTriggerTime = 0;
	bool canSpace = true;

	float prejumpInterval = 0.2f;
	float prejumpTime = 0;

	float jumpbackInterval = 0.3f;
	float jumpbackTime = 0;
	float jumpbackSpeedX = 200;
	float jumpbackSpeedY = 120;

	float knockbackInterval = 0.2f;
	float knockbackTime = 0;

	int knockbackDirection = 0;
	float knockbackSpeed = 415.f;

	float afterKnockbackInterval = 2.f;
	float afterKnockbackTime = 0;

	bool toBeKnockedBack = false;

	bool nextOrderReady = true;
	CatAction currentOrder;
	CatAction nextOrder;

	float numbInterval = 1.f;
	float numbTime = 0;
	int numbCount = 0;
	bool isNumb = false;
	int falseTime = 0;
	int minimumDistanceAtm = 9999;
	int headhitAmount = 1;

	float trackingNumber;

public:
	const static int BOUNCE_SPEED_Y = 430;

	bool resetStateIndicator;
	int canJumpBackRight = -1;
	int canJumpBackLeft = -1;
	int goingToJumpTo = 0;
	bool jumpAgainstWall = false;
	bool isGoingToDie = false;
	Door* lastKnownRainbowDoor;

	void Create(AvancezLib* system, Cat * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera, Mouse* mouse);
	void Init();
	void RoundInit();
	void Update(float dt);
	void Destroy();
	void Move(float x, float y);
	void Receive(Message* m);
	void ChangeSpeedX(float newVx);
	void ChangeSpeedY(float newVy);
	void WhenDoorOpen(GameEntity* door);
	void WhenDoorClose(GameEntity* door);
	void WhenHeadHit();
	void WhenDie();

	void clearOrder();
	void ThinkWhereToJump();
	void ThinkWhereToMove();
	void NumbMind();
	void NumbProgress(float dt);
	void ChangeDirection();

	bool isOnTheGround(int currentState) {
		switch (currentState) {
			case CatSpriteState::STATE_STAND:
			case CatSpriteState::STATE_WALK:
				return true;
			}
		return false;
	}

private:
	

};