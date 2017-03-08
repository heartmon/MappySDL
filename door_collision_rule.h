#pragma once

#include "collision_rule_interface.h"
#include "door.h"
#include "door_behavior_component.h"

class DoorCollisionRule : public CollisionRuleInterface {
	Door* gameEntity;
	SDL_Rect* camera;
	DoorBehaviorComponent* behaviorComponent;

public:
	void Create(Door* gameEntity, SDL_Rect* camera, DoorBehaviorComponent* behaviorComponent) {
		this->gameEntity = gameEntity;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}

	virtual void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_MOUSE) {

		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		if (withThisEntity->getName() == CLASS_MOUSE) {
			if (gameEntity->getCurrentStateType() != DoorSpriteState::STATE_DOOR_OPEN) {
				bool isCollided = checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox(camera));
				if (isCollided) {
					//SDL_Log("HIT DOOR");
				}
				return isCollided;
			}
			else {
				return 0;
			}
		}

		if (withThisEntity->getName() == CLASS_CAT) {
			if (gameEntity->getCurrentStateType() != DoorSpriteState::STATE_DOOR_OPEN) {
				bool isCollided = checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox());
				if (isCollided) {
					//SDL_Log("HIT DOOR");
				}
				return isCollided;
			}
			else {
				return 0;
			}
		}
	}
};