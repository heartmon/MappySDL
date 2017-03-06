#pragma once
#include "component.h"
#include "rainbow.h"

class RainbowBehaviorComponent : public Component {
	float rainbowSpeed = 180.f;
	Rainbow* rainbow;
	SDL_Rect* camera;
public:

	virtual void Create(AvancezLib* system, Rainbow * rainbow, SDL_Rect* camera) {
		Component::Create(system, rainbow, nullptr);
		this->camera = camera;
		this->rainbow = rainbow;
	}

	virtual void Update(float dt) {
		rainbow->horizontalPosition += dt*rainbowSpeed*rainbow->direction;
	
		// Set enable to false when out of the screen
		switch (rainbow->direction) {
			case GameEntity::LEFT:
				if (rainbow->horizontalPosition < 0 - rainbow->getSize()->w) {
					rainbow->enabled = false;
				}
			break;
			case GameEntity::RIGHT:
				if (rainbow->horizontalPosition > LEVEL_WIDTH) {
					rainbow->enabled = false;
				}
			break;
		}
	}
};