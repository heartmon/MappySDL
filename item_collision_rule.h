#pragma once
#pragma once

#include "collision_rule_interface.h"
#include "item.h"
#include "item_behavior_component.h"

class ItemCollisionRule : public CollisionRuleInterface {
	Item* gameEntity;
	SDL_Rect* camera;
	ItemBehaviorComponent* behaviorComponent;

public:
	void Create(Item* gameEntity, SDL_Rect* camera, ItemBehaviorComponent* behaviorComponent) {
		this->gameEntity = gameEntity;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}

	virtual void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_MOUSE) {
			gameEntity->enabled = false;
			gameEntity->Send(new Message(COLLECT_ITEM, gameEntity));
		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		if (withThisEntity->getName() == CLASS_MOUSE) {
			GameEntity::Box selfBox = self->getCollisionBox();
			GameEntity::Box mouseBox = withThisEntity->getCollisionBox(camera);

			selfBox.x = selfBox.x + selfBox.w / 2;
			selfBox.w = 1;

			mouseBox.x = mouseBox.x + mouseBox.w / 2;
			mouseBox.w = mouseBox.w / 2;

			bool isCollided = checkSquareCollision(selfBox, mouseBox);
			return isCollided;
		}
		return -1;
	}
};