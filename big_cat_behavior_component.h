#pragma once
#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <vector>
#include "component.h"
#include "big_cat_sprite_state.h"
#include "cat_behavior_component.h"

//enum BigCatAction { NOTHING, CAT_STAND, CAT_MOVE_LEFT, CAT_MOVE_RIGHT, CAT_JUMP_BACK_LEFT, CAT_JUMP_BACK_RIGHT, CAT_TOGGLE_DOOR };
class BigCat;
class Item;
class Mouse;
class Door;
class BigCatBehaviorComponent : public Component {
	SDL_Rect* camera;
	BigCat* gameEntity;
	Mouse* mouse;
	ObjectPool<Item>* itemPool;
	Item* currentTargetItem;

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
	float jumpbackSpeedY = 100;

	float knockbackInterval = 0.2f;
	float knockbackTime = 0;

	int knockbackDirection = 0;
	float knockbackSpeed = 400.f;

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

	float trackingNumber;

	// time
	float inItemTime = 0;
	float inItemInterval = 3.f;
	float inItemRandomOffset = 0;

	float inItemWithScoreTime = 0;
	float inItemWithScoreInterval = 2.f;


public:
	const static int BOUNCE_SPEED_Y = 430;

	bool resetStateIndicator;
	int canJumpBackRight = -1;
	int canJumpBackLeft = -1;
	int goingToJumpTo = 0;
	bool jumpAgainstWall = false;
	bool isGoingToDie = false;
	Door* lastKnownRainbowDoor;

	void Create(AvancezLib* system, BigCat * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera, ObjectPool<Item>* itemPool);
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

	void ThinkToPossessItem(Item* item);
	void FindNewTargetItem();
	void ForgetCurrentTargetItem();
	void ShowScore();

	bool isOnTheGround(int currentState) {
		switch (currentState) {
		case BigCatSpriteState::STATE_STAND:
		case BigCatSpriteState::STATE_WALK:
			return true;
		}
		return false;
	}

private:


};