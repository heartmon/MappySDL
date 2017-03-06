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
		if (m->getArg1()->getName() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;
		}
		/*if (m->getArg1()->getName() == CLASS_MOUSE) {
			behaviorComponent->removeLife();
		}*/
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		if (withThisEntity->getName() == CLASS_MOUSE) {
			if (gameEntity->getCurrentStateType() == RopeSpriteState::STATE_ROPE_STATIC) {
				GameEntity::Box selfBox = self->getCollisionBox();
				GameEntity::Box withBox = withThisEntity->getCollisionBox(camera);

				bool xCollided = selfBox.x <= withBox.x + withBox.w && selfBox.x + selfBox.w >= withBox.x;
				bool footCollided = (selfBox.y + selfBox.h / 2 - 5 <= withBox.y + withBox.h && selfBox.y + selfBox.h / 2 >= withBox.y + withBox.h - 5);
				bool headCollided = (selfBox.y + selfBox.h / 2 -5 <= withBox.y + 3 && selfBox.y + selfBox.h / 2 >= withBox.y + 1);
				bool yCollided = footCollided || headCollided;
				//bool isCollided = checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox(camera));
				bool isCollided = xCollided && yCollided;

				// head collided -> not remove life
				if (isCollided && headCollided) {
					return isCollided;
				}

				if (isCollided && behaviorComponent->getRopeLive() > 1) {
					behaviorComponent->removeLife();
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