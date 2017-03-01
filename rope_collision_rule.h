#pragma once

#include "collision_rule_interface.h"
#include "rope.h"
#include "rope_behavior_component.h"

class RopeCollisionRule : public CollisionRuleInterface {
	Rope* gameEntity;
	SDL_Rect* camera;
	RopeBehaviorComponent* behaviorComponent;

public:
	void Create(Rope* gameEntity, SDL_Rect* camera, RopeBehaviorComponent* behaviorComponent) {
		this->gameEntity = gameEntity;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}

	virtual void didHit(Message* m) {
		if (m->getArg1() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;
		}
		if (m->getArg1() == CLASS_MOUSE) {
			behaviorComponent->removeLife();
		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		if (withThisEntity->getName() == CLASS_MOUSE) {
			if (gameEntity->getCurrentStateType() == RopeSpriteState::STATE_ROPE_STATIC) {
				bool isCollided = checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox(camera));

				if (isCollided && behaviorComponent->getRopeLive() > 1) {
					return isCollided;
				}
				else if (isCollided && behaviorComponent->getRopeLive() <= 1 ) {
					behaviorComponent->ropeSeparated();
					return 0;
				}
				else {
					return isCollided;
				}
			}
			else {
				return 0;
			}
		}
	}
};