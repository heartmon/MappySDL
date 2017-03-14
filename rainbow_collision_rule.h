#pragma once
#include "collision_rule_interface.h"
#include "rainbow_behavior_component.h"

class RainbowCollisionRule : public CollisionRuleInterface {
	SDL_Rect* camera;
	Rainbow* rainbow;
	RainbowBehaviorComponent* behaviorComponent;

public:
	void Create(Rainbow* rainbow, SDL_Rect* camera, RainbowBehaviorComponent* behaviorComponent) {
		CollisionRuleInterface::Create(rainbow);
		this->gameEntity = rainbow;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_CAT) {
			behaviorComponent->numberOfCats++;
		}
	}

};