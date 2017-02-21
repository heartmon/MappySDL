#pragma once
#include "SDL.h"
#include "collision_rule_interface.h"
#include "mouse.h"
#include "tile.h"
#include "tile_sprite_state.h"

class MouseCollisionRule : public CollisionRuleInterface {
	Mouse* mouse;

public:
	void Create(Mouse* mouse) {
		this->mouse = mouse;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1() == CLASS_TILE) {
			mouse->isCollidedWithMap = true;
		}		
	}

	virtual int isCollided(GameEntity* withThisEntity) {
		//SDL_Log("Check from mouse specific");
		int result = -1;
		if (withThisEntity->getName() == CLASS_TILE) {
			Tile* tile = (Tile*)withThisEntity;
			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE) {
				//mouse->isCollidedWithMap = false;
				//SDL_Log("Collided on space");
				result = 0;
			}
			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_PATH) {
				if (
					mouse->verticalPosition + mouse->getSize()->h + 2 >= tile->verticalPosition
					&& (mouse->horizontalPosition >= tile->horizontalPosition && mouse->horizontalPosition <= tile->horizontalPosition + tile->getSize()->w)
					) {
					//collided
					SDL_Log("Collided on floor ja");
					result = 1;
				}
			}
		}
		return result;
	}
};
