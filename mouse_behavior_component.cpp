#include "mouse_behavior_component.h"
#include "game_entity.h"
#include "mouse_sprite_state.h"

void MouseBehaviorComponent::Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects) {
	Component::Create(system, go, game_objects);
}

void MouseBehaviorComponent::Update(float dt) {
	// Input behavior
	bool go_on = true;
	SDL_Event event;

	//AvancezLib::KeyStatus keys;
	//system->getKeyStatus(keys);
	//if (keys.right) {
	//	Move(dt * 0.1);
	//	gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
	//}
	//else if (keys.left)
	//	Move(-dt * 0.1);

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = false;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
				break;
			case SDLK_RIGHT:
				key.right = false;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
				break;
			}
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = true;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
				gameEntity->direction = GameEntity::LEFT;
				Move(-dt * 160.0f);
				break;
			case SDLK_RIGHT:
				key.right = true;
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_WALK);
				gameEntity->direction = GameEntity::RIGHT;
				Move(dt * 160.0f);
				//SDL_Log("GAME %f", gameEntity->horizontalPosition);
				break;
			}
		}

	}
}

void MouseBehaviorComponent::Move(float move)
{
	gameEntity->horizontalPosition += move;

	if (gameEntity->horizontalPosition > (640 - 32))
		gameEntity->horizontalPosition = 640 - 32;

	if (gameEntity->horizontalPosition < 0)
		gameEntity->horizontalPosition = 0;
}


void MouseBehaviorComponent::Receive(int message) {

}

void MouseBehaviorComponent::Destroy() {

}