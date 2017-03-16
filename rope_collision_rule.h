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
		GameEntity::Box withBox = withThisEntity->getCollisionBox(camera);
		if (withThisEntity->getName() == CLASS_CAT || withThisEntity->getName() == CLASS_BIG_CAT) {
			withBox = withThisEntity->getCollisionBox();
		}
		if (withThisEntity->getName() == CLASS_MOUSE || withThisEntity->getName() == CLASS_CAT || withThisEntity->getName() == CLASS_BIG_CAT) {
			if (gameEntity->getCurrentStateType() == RopeSpriteState::STATE_ROPE_STATIC || gameEntity->getCurrentStateType() == RopeSpriteState::STATE_ROPE_JUMP_ON) {
				GameEntity::Box selfBox = self->getCollisionBox();
				

				bool xCollided = selfBox.x <= withBox.x + withBox.w && selfBox.x + selfBox.w >= withBox.x;
				/*bool footCollided = (selfBox.y + selfBox.h / 2 - 20 <= withBox.y + withBox.h - 5 && selfBox.y + selfBox.h / 2 + 20 >= withBox.y + withBox.h - 10);*/
				bool footCollided = (withBox.y + withBox.h <= selfBox.y + selfBox.h && withBox.y + withBox.h + 10 >= selfBox.y + selfBox.h / 2);
				bool headCollided = (selfBox.y + selfBox.h / 2 -5 <= withBox.y + 3 && selfBox.y + selfBox.h / 2 >= withBox.y + 1);
				bool yCollided = (footCollided && withThisEntity->vy >= 0) || (headCollided  && withThisEntity->vy <= 0);
				bool isCollided = xCollided && yCollided;

				// head collided -> not remove life
				if (withThisEntity->getName() == CLASS_MOUSE) {
					if (isCollided && headCollided) {
						return isCollided;
					}

					if (isCollided && behaviorComponent->getRopeLive() > 1) {
						behaviorComponent->removeLife();
						return isCollided;
					}
					else if (isCollided && behaviorComponent->getRopeLive() <= 1) {
						behaviorComponent->ropeSeparated();
						return 0;
					}
					else {
						return isCollided;
					}
				}
				if (withThisEntity->getName() == CLASS_CAT || withThisEntity->getName() == CLASS_BIG_CAT) {
					if (isCollided) behaviorComponent->jumpOn();
					return isCollided;
				}
			}
			else {
				return 0;
			}
		}
	}
};