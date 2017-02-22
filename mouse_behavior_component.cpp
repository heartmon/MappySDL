#include "mouse_behavior_component.h"
#include "game_entity.h"
#include "mouse_sprite_state.h"
#include "global_constant.h"

float ax = 0;
float ay = 0;
float vx = 160.f;
float vy = 10;
float xold, yold, vxold, vyold;
int coeff = 10;
float x = 0;
float y = 0;

void MouseBehaviorComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera) {
	Component::Create(system, go, game_objects);

	this->camera = camera;
	cameraHorizontalFloat = camera->x;
	cameraVerticalFloat = camera->y;
}

void MouseBehaviorComponent::Update(float dt) {
	// Input behavior
	bool go_on = true;

	float g = 100.f;


	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);

	// input processing
	switch (gameEntity->getCurrentStateType()) {
		case MouseSpriteState::STATE_JUMP:
		case MouseSpriteState::STATE_PREJUMP:
			break;
		default:
			ProcessInput(keys, dt);
	}

	// default moving
	// X
	if (!gameEntity->isXCollidedWithMap) {
		if (isMovable) {
			Move(dt * gameEntity->direction * 160.f);
			isMovable = false;
		}
	}
	else {
		Move(0.5 * gameEntity->direction * -1);
		gameEntity->isXCollidedWithMap = false;
		isMovable = false;
	}
	// Y
	if (!gameEntity->isYCollidedWithMap) {
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_JUMP);
		gameEntity->verticalPosition += dt * gameEntity->vy;
		//gameEntity->verticalPosition += dt * -g * vy;
		gameEntity->vy += dt * g;
		SDL_Log("%f", gameEntity->vy);
	}

	if (gameEntity->isYCollidedWithMap) {
		if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP) {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
			gameEntity->vy = 100;
		}
		gameEntity->isYCollidedWithMap = false;
	}

	// Save previous values
	//xold = x;
	//yold = y;
	//vxold = vx;
	//vyold = vy;

	//// Update acceleration
	//ax = -(coeff*vxold) * (coeff*vyold);
	//ay = -(coeff*vyold) * (coeff*vyold) - 9.8;

	//vx = vxold + ax*dt;
	//vy = vyold + ay*dt;

	//// Update position
	//x = xold + vxold*dt + 0.5*ax*(dt * dt);
	//y = yold + vyold*dt + 0.5*ay*(dt * dt);

	//SDL_Log("Y: %f", y);

	// state processing

	//hopping
	if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PREJUMP) {
		if (gameEntity->arbitaryTrackingNumber == 0) {
			gameEntity->arbitaryTrackingNumber = camera->x;
		}
		//gameEntity->arbitaryTrackingNumber += (dt*gameEntity->direction);

		// 32 = one jump step
		if ((int)(camera->x - gameEntity->arbitaryTrackingNumber) <= 12) {
			gameEntity->verticalPosition += -dt * 50.f;
		}
		else {
			gameEntity->verticalPosition += dt * 50.f;
		}
		if (camera->x - gameEntity->arbitaryTrackingNumber >= 0 && camera->x - gameEntity->arbitaryTrackingNumber <= 16) {
			SDL_Log("%f", camera->x - gameEntity->arbitaryTrackingNumber);
			
			Move(dt*gameEntity->direction * 160.f);
		}
		else {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_JUMP);
			gameEntity->arbitaryTrackingNumber = 0;
		}

		return;
	}

	

	
}

void MouseBehaviorComponent::ProcessInput(AvancezLib::KeyStatus keys, float dt) {
	if (keys.right) {
		gameEntity->direction = GameEntity::RIGHT;
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK_RIGHT);
		isMovable = true;
	}

	if (keys.left) {
		gameEntity->direction = GameEntity::LEFT;
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
		isMovable = true;
	}

	if (!keys.left && !keys.right) {
		if (gameEntity->direction == GameEntity::RIGHT) {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND_RIGHT);
		}
		else {
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
		}
		isMovable = false;
	}
}

void MouseBehaviorComponent::Move(float move)
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
		cameraHorizontalFloat += move;
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
		gameEntity->horizontalPosition += move;
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