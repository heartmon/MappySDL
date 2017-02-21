#include "mouse_behavior_component.h"
#include "game_entity.h"
#include "mouse_sprite_state.h"
#include "global_constant.h"

void MouseBehaviorComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera) {
	Component::Create(system, go, game_objects);

	this->camera = camera;
	cameraHorizontalFloat = camera->x;
	cameraVerticalFloat = camera->y;
}

void MouseBehaviorComponent::Update(float dt) {
	// Input behavior
	bool go_on = true;

	AvancezLib::KeyStatus  keys;
	system->getKeyStatus(keys);

	if (keys.right) {
		Move(dt * 160.f);
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
	}

	if (keys.left) {
		Move(dt * -160.f);
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
	}

	if (!keys.left && !keys.right) {
		gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
	}

	// always affect by gravity
	if(!gameEntity->isCollidedWithMap)
		gameEntity->verticalPosition += dt * 60.0f;
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