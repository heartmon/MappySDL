#include "mouse_input_component.h"
#include "mouse.h"

MouseInputComponent::MouseInputComponent(AvancezLib* system) {
	//setup variable
	this->system = system;

	key.left = false;
	key.right = false;
}

void MouseInputComponent::Update(float dt, Mouse* mouse) {
	bool go_on = true;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = false;
				mouse->setCurrentState(Mouse::STATE_STAND);
				break;
			case SDLK_RIGHT:
				key.right = false;
				mouse->setCurrentState(Mouse::STATE_STAND);
				break;
			}
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = true;
				mouse->setCurrentState(Mouse::STATE_WALK);
				mouse->direction = GameEntity::LEFT;
				break;
			case SDLK_RIGHT:
				key.right = true;
				mouse->setCurrentState(Mouse::STATE_WALK);
				mouse->direction = GameEntity::RIGHT;
				break;
			}
		}

	}
}