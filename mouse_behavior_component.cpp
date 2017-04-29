#include "mouse_behavior_component.h"
#include <cmath>
#include "door.h"
#include "mouse.h"

void MouseBehaviorComponent::Create(AvancezLib* system, Mouse* go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera) {
	Component::Create(system, go, game_objects);
	this->gameEntity = go;

	this->camera = camera;
	
}

void MouseBehaviorComponent::Init() {
	cameraHorizontalFloat = camera->x;
	cameraVerticalFloat = camera->y;

	defaultVx = gameEntity->vx;
	defaultVy = gameEntity->vy;

	gameEntity->Send(new Message(UPDATE_LIFE, gameEntity, gameEntity->lives));
}

void MouseBehaviorComponent::RoundInit() {
	cameraHorizontalFloat = camera->x;
	cameraVerticalFloat = camera->y;

	gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
	gameEntity->Send(new Message(UPDATE_LIFE, gameEntity, gameEntity->lives));
}

void MouseBehaviorComponent::Update(float dt) {
	// Input behavior
	bool go_on = true;

	float g = 200.f;

	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);

	// sum
	if (!canSpace) {
		spaceTriggerTime += dt;
		if (spaceTriggerTime > spaceTriggerInterval) {
			canSpace = true;
			spaceTriggerTime = 0;
		}
	}

	// no behavior add all
	if (gameEntity->isStop) {
		return;
	}

	// cannot do anything
	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_DEAD) {
		deadTime += dt;
		if (deadTime > deadInterval) {
			deadTime = 0;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_DEAD_MOVING);
		}
		return;
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_DEAD_MOVING) {
		deadMovingTime += dt;
		if (deadMovingTime > deadMovingInterval) {
			deadMovingTime = 0;
			gameEntity->lives--;

			if (gameEntity->lives < 0) {
				gameEntity->Send(new Message(GAME_OVER, gameEntity));
			}
			else {
				gameEntity->Send(new Message(LIFE_DECREASE, gameEntity));
			}
		}
		return;
	}

	// input processing
	if (isOnTheGround(gameEntity->getCurrentStateType()) && gameEntity->getCurrentStateType() != MouseSpriteState::STATE_KNOCKBACK) {
		if (keys.right) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
			ChangeSpeedX(160.f);
		}

		if (keys.left) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
			ChangeSpeedX(160.f);
		}

		if (!keys.left && !keys.right) {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
			ChangeSpeedX(0.f);
		}

		gameEntity->ay = 0;
		goingToJumpTo = 0;
		resetStateIndicator = false;
		intheairDeadTime = 0;
	}

	if ((isOnTheGround(gameEntity->getCurrentStateType()) 
		//|| gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK)
		)
		&& gameEntity->getCurrentStateType() != MouseSpriteState::STATE_KNOCKBACK
		) {
		//space
		if (keys.fire) {
			if (canSpace) {
				gameEntity->Send(new Message(TOGGLE_DOOR, gameEntity));
				//canSpace = false;
			}
		}
	}

	// Update position
	float distanceX = dt * gameEntity->vx * gameEntity->direction;
	/*if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_KNOCKBACK) {
		distanceX = 0;
	}*/
	
	float distanceY = dt * gameEntity->vy;

	Move(distanceX, distanceY);

	// Update Velocity
	gameEntity->vy += dt * g * gameEntity->ay;

	// state processing
	// State = Knockback
	if (toBeKnockedBack) {
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_KNOCKBACK);
		toBeKnockedBack = false;
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_KNOCKBACK) {
		knockbackTime += dt;
		
		if (knockbackTime < knockbackInterval) {
			Move(dt*knockbackSpeed*knockbackDirection, 0);
		}
		if (knockbackTime > knockbackInterval + 0.01f ) {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
			knockbackTime = 0;
		}
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_INTHEAIR) {
		if (gameEntity->vy <= 0) {
			// process key when in the air
			if (keys.right) {
				gameEntity->direction = GameEntity::RIGHT;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::RIGHT;
			}

			if (keys.left) {
				gameEntity->direction = GameEntity::LEFT;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_PRE_JUMP_BACK);
				goingToJumpTo = GameEntity::LEFT;
			}
		}
		resetStateIndicator = false;
		ChangeSpeedX(0);

		intheairDeadTime += dt;
		if (intheairDeadTime > intheairDeadInterval) {
			intheairDeadTime = 0;
			isGoingToDie = true;
		}
		
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PRE_JUMP_BACK) {
		if (gameEntity->vy > 0) {
			goingToJumpTo = 0;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
		}
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK) {
		goingToJumpTo = 0; 
		// first setup value when enter this state
		if (!resetStateIndicator) {
			//SDL_Log("JUMP BACK");
			ChangeSpeedY(-jumpbackSpeedY);
			ChangeSpeedX(jumpbackSpeedX);
			gameEntity->ay = 4;
			resetStateIndicator = true;
			isGoingToDie = false;
		}

		return;
	}
	//hopping
	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PREJUMP) {
		if (!resetStateIndicator) {
			float jumpSpeed = 185;
			prejumpTime = 0;
			ChangeSpeedY(-140.f);
			ChangeSpeedX(jumpSpeed);
			gameEntity->ay = 7.5;
			resetStateIndicator = true;
			if (jumpAgainstWall) {
				ChangeSpeedY(-140.f/3 - 10);
				gameEntity->ay = 8.5;
			}
			yPosBeforePrejump = gameEntity->getCollisionBox(camera).y + 2;
		}

		if (yPosBeforePrejump <= gameEntity->getCollisionBox(camera).y) {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
			ChangeSpeedX(0);
			ChangeSpeedY(200);
			gameEntity->ay = 1;
			//SDL_Log("Change to in the air");
			resetStateIndicator = false;
			jumpAgainstWall = false;
			yPosBeforePrejump = 0;
		}
		return;
	}

	//SDL_Log("%d", prevState);

	
}

void MouseBehaviorComponent::Move(float distanceX, float distanceY)
{
	if (gameEntity->getSize() == NULL) {
		SDL_Log("Need getSize to be implemented!!");
		return;
	}
	SDL_Rect* entitySize = gameEntity->getSize();
	int centerCamera = (camera->w / 2) - (entitySize->w / 2);

	// change camera position
	int xPosInt = (int)gameEntity->horizontalPosition;
	if (xPosInt >= 0 && xPosInt < centerCamera && cameraHorizontalFloat <= 0) {

	}
	else if (xPosInt > centerCamera && xPosInt <= SCREEN_WIDTH - entitySize->w / 2 && cameraHorizontalFloat >= LEVEL_WIDTH - camera->w) {

	}
	else {
		cameraHorizontalFloat += distanceX;
	}

	//limited screen
	if (cameraHorizontalFloat < 0) {
		cameraHorizontalFloat = 0;
	}
	if (cameraHorizontalFloat > LEVEL_WIDTH - camera->w) {
		cameraHorizontalFloat = LEVEL_WIDTH - camera->w;
	}

	// If camera is on left-most or right-mose side
	if (cameraHorizontalFloat <= 0 || cameraHorizontalFloat >= LEVEL_WIDTH - camera->w) {
		gameEntity->horizontalPosition += distanceX;
	}

	// horizontal position bound screen
	if (gameEntity->horizontalPosition < 0) {
		gameEntity->horizontalPosition = 0;
	}
	if (gameEntity->horizontalPosition > SCREEN_WIDTH - gameEntity->getSize()->w) {
		gameEntity->horizontalPosition = SCREEN_WIDTH - gameEntity->getSize()->w;
	}

	// vertical position bound screen
	//if (gameEntity->verticalPosition < 0) {
	//	gameEntity->verticalPosition = 0;
	//}
	if (gameEntity->verticalPosition > SCREEN_HEIGHT - gameEntity->getSize()->h) {
		gameEntity->verticalPosition = SCREEN_HEIGHT - gameEntity->getSize()->h;
		gameEntity->Receive(new Message(MOUSE_DIE, gameEntity));
	}

	//set back to camera
	camera->x = cameraHorizontalFloat;

	// Y
	if (cameraVerticalFloat < 0) {
		cameraVerticalFloat = 0;
	}

	if (cameraVerticalFloat > LEVEL_HEIGHT - camera->h) {
		cameraVerticalFloat = LEVEL_HEIGHT - camera->h;
	}

	// If camera is on top-most or bottom-most side
	if (cameraVerticalFloat <=  0 || cameraVerticalFloat >= LEVEL_HEIGHT - camera->h) {
		gameEntity->verticalPosition += distanceY;
	}

}


void MouseBehaviorComponent::Receive(Message* m) {
	if (m->getMessageType() == MOUSE_TOGGLE_DOOR) {
		canSpace = false;
	}
}

void MouseBehaviorComponent::Destroy() {

}

void MouseBehaviorComponent::WhenDoorOpen(GameEntity* ent) {
	// check if mouse is at the range of the door >> knockback
	Door* door = (Door*)ent;
	GameEntity::Box mouseBox = gameEntity->getCollisionBox(camera);
	GameEntity::Box doorBox = door->getCollisionBox();
	float doorX = door->horizontalPosition;
	float doorW = door->getSize()->w;

	if (door->defaultState == DoorSpriteState::STATE_DOOR_LEFT) {
		// if inside door
		if (mouseBox.x <= doorX + doorW - doorW/4 && mouseBox.x + mouseBox.w >= doorX
			&& mouseBox.y <= doorBox.y + doorBox.h / 2 && mouseBox.y + mouseBox.h / 2 >= doorBox.y) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::LEFT;
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_KNOCKBACK);
		}
	}
	if (door->defaultState == DoorSpriteState::STATE_DOOR_RIGHT) {
		// if inside door
		if (mouseBox.x <= doorX + doorW && mouseBox.x + mouseBox.w >= doorX + doorW / 4
			&& mouseBox.y <= doorBox.y + doorBox.h / 2 && mouseBox.y + mouseBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::RIGHT;
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_KNOCKBACK);
		}
	}
}

void MouseBehaviorComponent::WhenDoorClose(GameEntity* ent) {
	Door* door = (Door*)ent;
	GameEntity::Box mouseBox = gameEntity->getCollisionBox(camera);
	GameEntity::Box doorBox = door->getCollisionBox();
	float doorX = door->horizontalPosition;
	float doorW = door->getSize()->w;

	if (door->defaultState == DoorSpriteState::STATE_DOOR_LEFT) {
		// if inside door
		if (mouseBox.x <= doorX + doorW && mouseBox.x + mouseBox.w >= doorX
			&& mouseBox.y <= doorBox.y + doorBox.h / 2 && mouseBox.y + mouseBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::RIGHT;
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_KNOCKBACK);
		}
	}
	if (door->defaultState == DoorSpriteState::STATE_DOOR_RIGHT) {
		// if inside door
		if (mouseBox.x <= doorX + doorW && mouseBox.x + mouseBox.w >= doorX
			&& mouseBox.y <= doorBox.y + doorBox.h / 2 && mouseBox.y + mouseBox.h / 2 >= doorBox.y
			) {
			//SDL_Log("KNOCKBACK!");
			toBeKnockedBack = true;
			knockbackDirection = GameEntity::LEFT;
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_KNOCKBACK);
		}
	}
}

void MouseBehaviorComponent::ChangeSpeedX(float newVx) {
	gameEntity->vx = newVx;
}
void MouseBehaviorComponent::ChangeSpeedY(float newVy) {
	gameEntity->vy = newVy;
}

void MouseBehaviorComponent::WhenHeadHit() {
	ChangeSpeedY(-gameEntity->vy);
	//gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
}

void MouseBehaviorComponent::WhenDie() {
	SDL_Log("DEAD :(");
	gameEntity->setCurrentStateType(MouseSpriteState::STATE_DEAD);
}