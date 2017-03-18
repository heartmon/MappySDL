#include "big_cat_behavior_component.h"
#include <cmath>
#include "door.h"
#include "big_cat.h"
#include "mouse.h"
#include "tile_sprite_state.h"
#include "mouse_sprite_state.h"
#include "item.h"

void BigCatBehaviorComponent::Create(AvancezLib* system, BigCat* go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera, ObjectPool<Item>* itemPool) {
	Component::Create(system, go, game_objects);
	this->gameEntity = go;
	this->camera = camera;
	this->itemPool = itemPool;
}

void BigCatBehaviorComponent::Init() {
	defaultVx = gameEntity->vx;
	defaultVy = gameEntity->vy;

	currentOrder = NOTHING;
	nextOrder = NOTHING;

	currentTargetItem = NULL;
}

void BigCatBehaviorComponent::RoundInit() {
	gameEntity->setCurrentStateType(BigCatSpriteState::STATE_STAND);
	currentTargetItem = NULL;
}

void BigCatBehaviorComponent::Update(float dt) {
	bool go_on = true;
	float g = 200.f;
	
	// not moving the same way, not collide with anything now
	if (gameEntity->isStop) {
		Move(dt*gameEntity->direction*180.f, 0);

		if (gameEntity->horizontalPosition < 0 || gameEntity->horizontalPosition > LEVEL_WIDTH) {
			gameEntity->enabled = false;

			// send message to spawn a new one....
			gameEntity->Send(new Message(CAT_DIE, gameEntity));
		}
		return;
	}

	// sum
	spaceTriggerTime += dt;

	if (spaceTriggerTime > spaceTriggerInterval) {
		canSpace = true;
		spaceTriggerTime = 0;
	}

	// not check other state until not in IN_ITEM state
	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_IN_ITEM) {
		inItemTime += dt;
		if (inItemTime > inItemInterval + inItemRandomOffset) {
			inItemTime = 0;
			inItemRandomOffset = (rand() % 6)/2 - 2;
			ForgetCurrentTargetItem();
			FindNewTargetItem();
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_STAND);
		}

		return;
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_IN_ITEM_WITH_SCORE) {
		inItemWithScoreTime += dt;
		if (inItemWithScoreTime > inItemWithScoreInterval) {
			inItemWithScoreTime = 0;
			ForgetCurrentTargetItem();
			FindNewTargetItem();
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_STAND);
		}

		return;
	}


	FindNewTargetItem();



	// input processing
	if (isOnTheGround(gameEntity->getCurrentStateType()) &&
		(gameEntity->getCurrentStateType() != BigCatSpriteState::STATE_KNOCKBACK && gameEntity->getCurrentStateType() != BigCatSpriteState::STATE_AFTER_KNOCKBACK)
		) {
		if (currentOrder == CAT_MOVE_RIGHT) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_WALK);
			ChangeSpeedX(160.f);
		}

		if (currentOrder == CAT_MOVE_LEFT) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_WALK);
			ChangeSpeedX(160.f);
		}

		////space
		//if (keys.fire) {
		//	if (canSpace) {
		//		gameEntity->Send(new Message(TOGGLE_DOOR, gameEntity));
		//		canSpace = false;
		//	}
		//}

		gameEntity->ay = 0;
		goingToJumpTo = 0;
		resetStateIndicator = false;
	}

	// Update position
	float distanceX = dt * gameEntity->vx * gameEntity->direction;
	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_KNOCKBACK || gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_AFTER_KNOCKBACK) {
		distanceX = 0;
	}


	float distanceY = dt * gameEntity->vy;
	//if (gameEntity->vx > 160) {
	//	SDL_Log("????");
	//}
	Move(distanceX, distanceY);

	/*char[20] msg;
	sprintf(msg, "VX: %f", gameEntity->vx);
	system->drawText(0, 0)*/

	// Update Velocity
	gameEntity->vy += dt * g * gameEntity->ay;

	// state processing
	// State = Knockback
	if (toBeKnockedBack) {
		gameEntity->setCurrentStateType(BigCatSpriteState::STATE_KNOCKBACK);
		toBeKnockedBack = false;
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_KNOCKBACK) {
		knockbackTime += dt;
		knockbackInterval = 0.2f;
		if (knockbackTime < knockbackInterval) {
			Move(dt*knockbackSpeed*knockbackDirection, 0);
		}
		if (knockbackTime > knockbackInterval + 1.f) {
			knockbackTime = 0;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_AFTER_KNOCKBACK);
		}
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_AFTER_KNOCKBACK) {
		afterKnockbackTime += dt;
		if (afterKnockbackTime > afterKnockbackInterval) {
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_STAND);
			afterKnockbackTime = 0;
		}
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_INTHEAIR) {
		if (gameEntity->vy <= 0) {
			// process key when in the air
			if (currentOrder == CAT_JUMP_BACK_RIGHT) {
				gameEntity->direction = GameEntity::RIGHT;
				gameEntity->setCurrentStateType(BigCatSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::RIGHT;
			}

			if (currentOrder == CAT_JUMP_BACK_LEFT) {
				gameEntity->direction = GameEntity::LEFT;
				gameEntity->setCurrentStateType(BigCatSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::LEFT;
			}
		}
		resetStateIndicator = false;
		ChangeSpeedX(0);

		ThinkWhereToJump();
	}



	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_PRE_JUMP_BACK) {
		if (gameEntity->vy > 0) {
			goingToJumpTo = 0;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_INTHEAIR);
		}
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_JUMP_BACK) {
		goingToJumpTo = 0;
		if (!resetStateIndicator) {
			ChangeSpeedY(-jumpbackSpeedY);
			ChangeSpeedX(jumpbackSpeedX);
			gameEntity->ay = 3.5;
			resetStateIndicator = true;
			isGoingToDie = false;
		}
		return;
	}
	//hopping
	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_PREJUMP) {
		if (!resetStateIndicator) {
			//SDL_Log("Pre jump");
			float jumpSpeed = 100;
			prejumpTime = 0;
			ChangeSpeedY(-100.f);
			ChangeSpeedX(jumpSpeed);
			gameEntity->ay = 3.5;
			resetStateIndicator = true;
			if (jumpAgainstWall) {
				ChangeSpeedY(0);
			}
			isGoingToDie = true;
		}

		prejumpTime += dt;
		if (prejumpTime <= 0.2 && (prejumpTime <= 0.1 || !jumpAgainstWall)) {
			Move(dt*gameEntity->vx*gameEntity->direction, 0);
		}
		else {
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_INTHEAIR);
			ChangeSpeedX(0);
			ChangeSpeedY(200);
			gameEntity->ay = 1;
			//SDL_Log("Change to in the air");
			resetStateIndicator = false;
			jumpAgainstWall = false;
		}

		return;
	}

	if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_STAND) {
		clearOrder();
		ThinkWhereToMove();
	}

	//SDL_Log("%d", prevState);


	NumbProgress(dt);
	if (nextOrder != NOTHING) {
		currentOrder = nextOrder;
		nextOrder = NOTHING;
	}

}

void BigCatBehaviorComponent::ThinkWhereToMove() {
	if (currentTargetItem == NULL) return;
	if (nextOrder != NOTHING) {
		return;
	}
	clearOrder();
	//if (isNumb) {
	//	return;
	//}

	GameEntity::Box itemBox = currentTargetItem->getCollisionBox();
	GameEntity::Box catBox = gameEntity->getCollisionBox();

	int mouseBlockYPos = ceil(itemBox.y) / TileSpriteState::TILE_HEIGHT;
	int catBlockYPos = ceil(catBox.y) / TileSpriteState::TILE_HEIGHT;

	switch (gameEntity->getCurrentStateType()) {
	case BigCatSpriteState::STATE_STAND:
	case BigCatSpriteState::STATE_WALK:
		if (gameEntity->horizontalPosition < itemBox.x) {
			//SDL_Log("Cat will move right");
			nextOrder = CAT_MOVE_RIGHT;
		}
		else {
			//SDL_Log("Cat will move left");
			nextOrder = CAT_MOVE_LEFT;
		}
		break;
	}
}

void BigCatBehaviorComponent::ChangeDirection() {
	if (currentOrder == CAT_MOVE_RIGHT) {
		//SDL_Log("From right to left");
		nextOrder = CAT_MOVE_LEFT;
	}
	else {
		//SDL_Log("From left to right");
		nextOrder = CAT_MOVE_RIGHT;
	}
}

void BigCatBehaviorComponent::NumbMind() {
	isNumb = true;
	numbTime = 0;
	numbCount++;
	SDL_Log("Numb");
}

void BigCatBehaviorComponent::NumbProgress(float dt) {
	if (isNumb) {
		numbTime += dt;
		if (numbTime > numbInterval) {
			isNumb = false;
		}
	}
}

void BigCatBehaviorComponent::ThinkWhereToJump() {
	if (currentTargetItem == NULL) return;
	clearOrder();
	if (isNumb) {
		return;
	}

	GameEntity::Box itemBox = currentTargetItem->getCollisionBox();
	GameEntity::Box catBox = gameEntity->getCollisionBox();

	int itemBlockYPos = ceil(itemBox.y) / TileSpriteState::TILE_HEIGHT;
	int catBlockYPos = ceil(catBox.y) / TileSpriteState::TILE_HEIGHT;
	int maximumPos = ceil(LEVEL_HEIGHT) / TileSpriteState::TILE_HEIGHT - 2;
	
	int blockPos = -1;
	if (lastKnownRainbowDoor != NULL) {
		GameEntity::Box doorBox = lastKnownRainbowDoor->getCollisionBox();
		blockPos = ceil(doorBox.y) / TileSpriteState::TILE_HEIGHT + 1;
	}

	switch (gameEntity->getCurrentStateType()) {
	case BigCatSpriteState::STATE_INTHEAIR:
		if (gameEntity->vy > 0) {
			return;
		}
		
		//SDL_Log("%d, %d", maximumPos, catBlockYPos);

		bool yDistance;
		//know the block
		if (blockPos != -1 && blockPos >= catBlockYPos - 2 && blockPos != maximumPos) {
			yDistance = true;
			lastKnownRainbowDoor = NULL;
		}
		else if (blockPos == maximumPos && catBlockYPos + 1 <= blockPos) {
			//SDL_Log("Block door is bottom floor");
			yDistance = true;
		}
		else if (blockPos == -1) {
			yDistance = catBlockYPos == itemBlockYPos || catBlockYPos - 1 == itemBlockYPos;
			if (!yDistance) falseTime++;;
		}
		if (yDistance || falseTime == 50) {
			if (gameEntity->horizontalPosition < itemBox.x) {
				//SDL_Log("Cat will jump right");
				nextOrder = CAT_JUMP_BACK_RIGHT;
			}
			else {
				//SDL_Log("Cat will jump left");
				nextOrder = CAT_JUMP_BACK_LEFT;
			}
			lastKnownRainbowDoor = NULL;
			numbCount = 0;
			falseTime = 0;
		}
		else {
			
		}
		break;
	}
}

void BigCatBehaviorComponent::Destroy() {

}

void BigCatBehaviorComponent::WhenDoorOpen(GameEntity* ent) {
	// check if mouse is at the range of the door >> knockback
	Door* door = (Door*)ent;
	GameEntity::Box catBox = gameEntity->getCollisionBox();
	GameEntity::Box doorBox = door->getCollisionBox();
	float doorX = door->horizontalPosition;
	float doorW = door->getSize()->w;
	//SDL_Log("================================");
	if (door->defaultState == DoorSpriteState::STATE_DOOR_LEFT) {
		// if inside door
		if (catBox.x <= doorX + doorW - doorW / 4 && catBox.x + catBox.w >= doorX
			&& catBox.y <= doorBox.y + doorBox.h / 2 && catBox.y + catBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("CATTTT KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::LEFT;
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_KNOCKBACK);
		}
	}
	if (door->defaultState == DoorSpriteState::STATE_DOOR_RIGHT) {
		// if inside door
		if (catBox.x <= doorX + doorW && catBox.x + catBox.w >= doorX + doorW / 4
			&& catBox.y <= doorBox.y + doorBox.h / 2 && catBox.y + catBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::RIGHT;
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_KNOCKBACK);
		}
	}
}

void BigCatBehaviorComponent::WhenDoorClose(GameEntity* ent) {
	Door* door = (Door*)ent;
	GameEntity::Box catBox = gameEntity->getCollisionBox();
	GameEntity::Box doorBox = door->getCollisionBox();
	float doorX = door->horizontalPosition;
	float doorW = door->getSize()->w;

	if (door->defaultState == DoorSpriteState::STATE_DOOR_LEFT) {
		// if inside door
		if (catBox.x <= doorX + doorW - doorW / 4 && catBox.x + catBox.w >= doorX
			&& catBox.y <= doorBox.y + doorBox.h / 2 && catBox.y + catBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_KNOCKBACK);
		}
	}
	if (door->defaultState == DoorSpriteState::STATE_DOOR_RIGHT) {
		// if inside door
		if (catBox.x <= doorX + doorW && catBox.x + catBox.w >= doorX + doorW / 4
			&& catBox.y <= doorBox.y + doorBox.h / 2 && catBox.y + catBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::LEFT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_KNOCKBACK);
		}
	}
}

void BigCatBehaviorComponent::Move(float x, float y) {
	gameEntity->horizontalPosition += x;
	gameEntity->verticalPosition += y;
}

void BigCatBehaviorComponent::ChangeSpeedX(float newVx) {
	gameEntity->vx = newVx;
}
void BigCatBehaviorComponent::ChangeSpeedY(float newVy) {
	gameEntity->vy = newVy;
}

void BigCatBehaviorComponent::WhenHeadHit() {
	ChangeSpeedY(-gameEntity->vy);
	//gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
}

void BigCatBehaviorComponent::WhenDie() {
	//SDL_Log("DEAD :(");
	//gameEntity->setCurrentStateType(CatSpriteState::STATE_DEAD);
}

void BigCatBehaviorComponent::Receive(Message* m) {

	if (m->getMessageType() == DOOR_OPEN) {
		WhenDoorOpen(m->getArg1());
	}
	if (m->getMessageType() == DOOR_CLOSE) {
		WhenDoorClose(m->getArg1());
	}
	if (m->getMessageType() == HEAD_HIT) {
		WhenHeadHit();
	}

}

void BigCatBehaviorComponent::clearOrder() {
	currentOrder = NOTHING;
	nextOrder = NOTHING;
	nextOrderReady = true;
}

void BigCatBehaviorComponent::ThinkToPossessItem(Item* item) {
	if (currentTargetItem == NULL) return;
	if (currentTargetItem != item) return;
	if (gameEntity->getCurrentStateType() != BigCatSpriteState::STATE_WALK) return;

	int chance = (rand() % 2) + 1;

	if (chance == 2) {
		SDL_Log("-----------------BigCat => Go in to the item!!");
		gameEntity->setCurrentStateType(BigCatSpriteState::STATE_IN_ITEM);
	}
	else {
		SDL_Log("-----------------BigCat => Change my mind :D");
		ForgetCurrentTargetItem();
	}
}

void BigCatBehaviorComponent::FindNewTargetItem() {
	// Set current target item
	if (currentTargetItem != NULL && !currentTargetItem->enabled) {
		ForgetCurrentTargetItem();
	}
	if (currentTargetItem == NULL) {
		currentTargetItem = itemPool->SelectRandom();
		SDL_Log("Item been targeted, %d", currentTargetItem->getId());
	}
}

void BigCatBehaviorComponent::ForgetCurrentTargetItem() {
	currentTargetItem = NULL;
}

void BigCatBehaviorComponent::ShowScore() {
	gameEntity->direction = GameEntity::LEFT;
	gameEntity->setCurrentStateType(BigCatSpriteState::STATE_IN_ITEM_WITH_SCORE);
}