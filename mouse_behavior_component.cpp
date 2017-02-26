#include "mouse_behavior_component.h"
#include "game_entity.h"
#include "global_constant.h"
#include <cmath>

void MouseBehaviorComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera) {
	Component::Create(system, go, game_objects);

	this->camera = camera;
	
}

void MouseBehaviorComponent::Init() {
	cameraHorizontalFloat = camera->x;
	cameraVerticalFloat = camera->y;

	defaultVx = gameEntity->vx;
	defaultVy = gameEntity->vy;
}

void MouseBehaviorComponent::Update(float dt) {
	prevState = gameEntity->getCurrentStateType();

	// Input behavior
	bool go_on = true;

	float g = 200.f;

	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);

	// input processing
	if (isOnTheGround(gameEntity->getCurrentStateType())) {
		if (keys.right) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK_RIGHT);
			ChangeSpeedX(160.f);
		}

		if (keys.left) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
			ChangeSpeedX(-160.f);
		}

		if (!keys.left && !keys.right) {
			if (gameEntity->direction == GameEntity::RIGHT) {
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND_RIGHT);
			}
			else {
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
			}
			ChangeSpeedX(0.f);
		}

		gameEntity->ay = 0;
		goingToJumpTo = 0;
		resetStateIndicator = false;
	}

	// Update position
	float distanceX = dt * gameEntity->vx;
	float distanceY = dt * gameEntity->vy;
	Move(distanceX, distanceY);

	// Update Velocity
	gameEntity->vy += dt * g * gameEntity->ay;


	// state processing
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
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PRE_JUMP_BACK) {
		if (gameEntity->vy > 0) {
			goingToJumpTo = 0;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
		}
	}

	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK) {
		goingToJumpTo = 0;
		if (!resetStateIndicator) {
			SDL_Log("JUMP BACK");
			float jumpSpeed = 100;
			trackingNumber = 0;
			ChangeSpeedY(-100.f);
			ChangeSpeedX(gameEntity->direction*jumpSpeed);
			gameEntity->ay = 3.5;
			resetStateIndicator = true;
		}

		trackingNumber += dt;
		if (trackingNumber <= 0.3) {
			Move(dt*gameEntity->vx, 0);
		}
		else {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
			ChangeSpeedX(0);
			ChangeSpeedY(200);
			gameEntity->ay = 1;
			SDL_Log("Change to in the air");
			resetStateIndicator = false;
		}

		return;
	}
	//hopping
	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PREJUMP) {
		if (!resetStateIndicator) {
			SDL_Log("Pre jump");
			float jumpSpeed = 100;
			trackingNumber = 0;
			ChangeSpeedY(-100.f);
			ChangeSpeedX(gameEntity->direction*jumpSpeed);
			gameEntity->ay = 3.5;
			resetStateIndicator = true;
			if (jumpAgainstWall) {
				ChangeSpeedY(0);
			}
		}

		trackingNumber += dt;
		if ( trackingNumber <= 0.2 && (trackingNumber <= 0.1 || !jumpAgainstWall)) {
			Move(dt*gameEntity->vx, 0);
		}
		else {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
			ChangeSpeedX(0);
			ChangeSpeedY(200);
			gameEntity->ay = 1;
			SDL_Log("Change to in the air");
			resetStateIndicator = false;
			jumpAgainstWall = false;
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
	if (gameEntity->horizontalPosition > SCREEN_WIDTH - entitySize->w) {
		gameEntity->horizontalPosition = SCREEN_WIDTH - entitySize->w;
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
	if (cameraVerticalFloat <= 0 || cameraVerticalFloat >= LEVEL_HEIGHT - camera->h) {
		gameEntity->verticalPosition += distanceY;
	}

}


void MouseBehaviorComponent::Receive(int message) {

}

void MouseBehaviorComponent::Destroy() {

}

void MouseBehaviorComponent::MoveCamera() {
	//Center the camera over the dot
	//camera->x = (x + 32 / 2) - 32 / 2;
	//camera->y = (y + 32 / 2) - 32 / 2;

	//Keep the camera in bounds
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w)
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h)
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}