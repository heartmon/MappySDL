#include "cat_behavior_component.h"
#include <cmath>
#include "door.h"
#include "cat.h"
#include "mouse.h"
#include "tile_sprite_state.h"
#include "mouse_sprite_state.h"

void CatBehaviorComponent::Create(AvancezLib* system, Cat* go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera, Mouse* mouse) {
	Component::Create(system, go, game_objects);
	this->gameEntity = go;
	this->cat = go;
	this->camera = camera;
	this->mouse = mouse;
}

void CatBehaviorComponent::Init() {
	defaultVx = gameEntity->vx;
	defaultVy = gameEntity->vy;

	currentOrder = NOTHING;
	nextOrder = NOTHING;

	toBeKnockedBack = false;
}

void CatBehaviorComponent::RoundInit() {
	gameEntity->setCurrentStateType(CatSpriteState::STATE_STAND);

	headhitAmount = 1;
	minimumDistanceAtm = 9999;
}

void CatBehaviorComponent::Update(float dt) {
	bool go_on = true;
	float g = 200.f;

	if (nextOrder != NOTHING) {
		currentOrder = nextOrder;
		nextOrder = NOTHING;
	}

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

	// input processing
	if (isOnTheGround(gameEntity->getCurrentStateType()) && 
		(gameEntity->getCurrentStateType() != CatSpriteState::STATE_KNOCKBACK && gameEntity->getCurrentStateType() != CatSpriteState::STATE_AFTER_KNOCKBACK)
		) {
		if (currentOrder == CAT_MOVE_RIGHT || currentOrder == CAT_JUMP_BACK_RIGHT) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_WALK);
			ChangeSpeedX(catWalkingSpeed*gameSpeed);
		}

		if (currentOrder == CAT_MOVE_LEFT || currentOrder == CAT_JUMP_BACK_LEFT) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_WALK);
			ChangeSpeedX(catWalkingSpeed*gameSpeed);
		}

		//SDL_Log("%d", currentOrder);

		gameEntity->ay = 0;
		goingToJumpTo = 0;
		resetStateIndicator = false;
	}

	// Update position
	float distanceX = dt * gameEntity->vx * gameEntity->direction;
	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_KNOCKBACK || gameEntity->getCurrentStateType() == CatSpriteState::STATE_AFTER_KNOCKBACK) {
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
		gameEntity->setCurrentStateType(CatSpriteState::STATE_KNOCKBACK);
		toBeKnockedBack = false;
	}

	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_KNOCKBACK) {
		knockbackTime += dt;
		knockbackInterval = 0.2f;
		if (knockbackTime < knockbackInterval) {
			Move(dt*knockbackSpeed*knockbackDirection, 0);
		}
		if (knockbackTime > knockbackInterval + 1.f) {
			knockbackTime = 0;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_AFTER_KNOCKBACK);
		}
	}

	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_AFTER_KNOCKBACK) {
		afterKnockbackTime += dt;
		if (afterKnockbackTime > afterKnockbackInterval) {
			gameEntity->setCurrentStateType(CatSpriteState::STATE_STAND);
			afterKnockbackTime = 0;
		}
	}

	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_INTHEAIR) {
		if (gameEntity->vy <= 0) {
			// process key when in the air
			if (currentOrder == CAT_JUMP_BACK_RIGHT) {
				gameEntity->direction = GameEntity::RIGHT;
				gameEntity->setCurrentStateType(CatSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::RIGHT;
			}

			if (currentOrder == CAT_JUMP_BACK_LEFT) {
				gameEntity->direction = GameEntity::LEFT;
				gameEntity->setCurrentStateType(CatSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::LEFT;
			}
		}
		resetStateIndicator = false;
		ChangeSpeedX(0);

		ThinkWhereToJump();
	}



	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_PRE_JUMP_BACK) {
		if (gameEntity->vy > 0) {
			goingToJumpTo = 0;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_INTHEAIR);
		}
	}

	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_JUMP_BACK) {
		goingToJumpTo = 0;
		if (!resetStateIndicator) {
			ChangeSpeedY(-jumpbackSpeedY);
			ChangeSpeedX(jumpbackSpeedX);
			gameEntity->ay = 3.5;
			resetStateIndicator = true;
			isGoingToDie = false;
		}

		if (gameEntity->direction == GameEntity::RIGHT) {
			currentOrder = CAT_MOVE_RIGHT;
		}
		else {
			currentOrder = CAT_MOVE_LEFT;
		}

		return;
	}
	//hopping
	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_PREJUMP) {
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
			currentOrder = NOTHING;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_INTHEAIR);
			ChangeSpeedX(0);
			ChangeSpeedY(200);
			gameEntity->ay = 1;
			//SDL_Log("Change to in the air");
			resetStateIndicator = false;
			jumpAgainstWall = false;
		}

		return;
	}

	if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_STAND) {
		currentOrder = NOTHING;
		ThinkWhereToMove();
	}

	//SDL_Log("%d", prevState);


	NumbProgress(dt);
	/*if (nextOrder != NOTHING) {
		currentOrder = nextOrder;
		nextOrder = NOTHING;
	}*/

}

void CatBehaviorComponent::ThinkWhereToMove() {
	currentOrder = NOTHING;
	if (currentOrder != NOTHING) {
		return;
	}
	//if (isNumb) {
	//	return;
	//}

	GameEntity::Box mouseBox = mouse->getCollisionBox(camera);
	GameEntity::Box catBox = cat->getCollisionBox();

	int mouseBlockYPos = ceil(mouseBox.y) / TileSpriteState::TILE_HEIGHT;
	int catBlockYPos = ceil(catBox.y) / TileSpriteState::TILE_HEIGHT;

	switch (gameEntity->getCurrentStateType()) {
	case CatSpriteState::STATE_STAND:
	case CatSpriteState::STATE_WALK:
		if (gameEntity->horizontalPosition < mouseBox.x) {
			//SDL_Log("Cat will move right");
			currentOrder = CAT_MOVE_RIGHT;
		}
		else {
			//SDL_Log("Cat will move left");
			currentOrder = CAT_MOVE_LEFT;
		}
		break;
	}
}

void CatBehaviorComponent::ChangeDirection() {
	if(currentOrder == CAT_MOVE_RIGHT) {
		//SDL_Log("From right to left");
		currentOrder = CAT_MOVE_LEFT;
	}
	else {
		//SDL_Log("From left to right");
		currentOrder = CAT_MOVE_RIGHT;
	}
}

void CatBehaviorComponent::NumbMind() {
	isNumb = true;
	numbTime = 0;
	numbCount++;
	SDL_Log("Numb");
}

void CatBehaviorComponent::NumbProgress(float dt) {
	if (isNumb) {
		numbTime += dt;
		if (numbTime > numbInterval) {
			isNumb = false;
		}
	}
}

void CatBehaviorComponent::ThinkWhereToJump() {
	if (isNumb) {
		return;
	}
	if (currentOrder != NOTHING) {
		return;
	}

	int maximumPos = ceil(LEVEL_HEIGHT) / TileSpriteState::TILE_HEIGHT - 2;

	int blockPos = -1;
	if (lastKnownRainbowDoor != NULL) {
		GameEntity::Box doorBox = lastKnownRainbowDoor->getCollisionBox();
		blockPos = ceil(doorBox.y) / TileSpriteState::TILE_HEIGHT + 1;
	}

	GameEntity::Box mouseBox = mouse->getCollisionBox(camera);
	GameEntity::Box catBox = cat->getCollisionBox();

	int mouseBlockYPos = ceil(mouseBox.y) / TileSpriteState::TILE_HEIGHT;
	int catBlockYPos = ceil(catBox.y) / TileSpriteState::TILE_HEIGHT;

	//SDL_Log("%d", abs(catBlockYPos - mouseBlockYPos));
	if (abs(catBlockYPos - mouseBlockYPos) < minimumDistanceAtm) {
		minimumDistanceAtm = abs(catBlockYPos - mouseBlockYPos);
	}

	switch (gameEntity->getCurrentStateType()) {
	case CatSpriteState::STATE_INTHEAIR:
		if (gameEntity->vy > 0) {
			return;
		}
		//SDL_Log("%d, %d, %d, %d", catBlockYPos, mouseBlockYPos, blockPos, maximumPos);
		bool yDistance = false;
		int preventLoopCheck = (200 + ((rand() % 20) - 30)) / headhitAmount;
		if (abs(mouseBox.x - catBox.x) > SCREEN_WIDTH) {
			int randomFloor = (rand() % maximumPos);
			yDistance = randomFloor - 1 >= catBlockYPos && randomFloor + 1 <= catBlockYPos;
			//SDL_Log("Case 1");
		}
		else if (blockPos != -1 && blockPos >= catBlockYPos - 2 && blockPos != maximumPos) {
			//SDL_Log("Case 2");
			yDistance = true;
			lastKnownRainbowDoor = NULL;
		}
		else if (blockPos == maximumPos && catBlockYPos + 1 <= blockPos) {
			//SDL_Log("Block door is bottom floor");
			yDistance = true;
		}
		else if (minimumDistanceAtm >= 1 && (falseTime >= preventLoopCheck || preventLoopCheck < 0)) {
			yDistance = true;
		}
		else if (blockPos==-1){
			//SDL_Log("Follow case");
			yDistance = catBlockYPos == mouseBlockYPos || catBlockYPos - (rand()%2) == mouseBlockYPos;
			bool notCareMouseState = true; //flag, whether the first or second condition's happened
			bool stateOfMouse = mouse->getCurrentStateType() != MouseSpriteState::STATE_INTHEAIR;
			if ((rand() % 2) + 1 + numbCount == 2)
				notCareMouseState = false;
			//
			if (!notCareMouseState && !stateOfMouse) {
				NumbMind();
				//SDL_Log("Don't know what to do~~ %d", numbCount);
				//falseTime = 0;
				return;
			}
			//if (!yDistance) falseTime++;
		}

		//
		if (yDistance) {
			if (gameEntity->horizontalPosition < mouseBox.x) {
				//SDL_Log("Cat will jump right");
				currentOrder = CAT_JUMP_BACK_RIGHT;
			}
			else {
				//SDL_Log("Cat will jump left");
				currentOrder = CAT_JUMP_BACK_LEFT;
			}

			numbCount = 0;
			falseTime = 0;
			lastKnownRainbowDoor = NULL;
			minimumDistanceAtm = 9999;
			headhitAmount = 1;
		}
		else {
			falseTime++;
		}
		break;
	}
}

void CatBehaviorComponent::Destroy() {

}

void CatBehaviorComponent::WhenDoorOpen(GameEntity* ent) {
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
			gameEntity->setCurrentStateType(CatSpriteState::STATE_KNOCKBACK);
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
			gameEntity->setCurrentStateType(CatSpriteState::STATE_KNOCKBACK);
		}
	}
}

void CatBehaviorComponent::WhenDoorClose(GameEntity* ent) {
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
			gameEntity->setCurrentStateType(CatSpriteState::STATE_KNOCKBACK);
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
			gameEntity->setCurrentStateType(CatSpriteState::STATE_KNOCKBACK);
		}
	}
}

void CatBehaviorComponent::Move(float x, float y) {
	gameEntity->horizontalPosition += x;
	gameEntity->verticalPosition += y;
}

void CatBehaviorComponent::ChangeSpeedX(float newVx) {
	gameEntity->vx = newVx;
}
void CatBehaviorComponent::ChangeSpeedY(float newVy) {
	gameEntity->vy = newVy;
}

void CatBehaviorComponent::WhenHeadHit() {
	ChangeSpeedY(-gameEntity->vy);
	headhitAmount++;
	//gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
}

void CatBehaviorComponent::WhenDie() {
	//SDL_Log("DEAD :(");
	//gameEntity->setCurrentStateType(CatSpriteState::STATE_DEAD);
}

void CatBehaviorComponent::Receive(Message* m) {
	
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

void CatBehaviorComponent::clearOrder() {
	nextOrder = NOTHING;
	nextOrderReady = true;
}