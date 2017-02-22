#pragma once
#include "SDL.h"
#include "collision_rule_interface.h"
#include "mouse.h"
#include "tile.h"
#include "tile_sprite_state.h"

class MouseCollisionRule : public CollisionRuleInterface {
	Mouse* mouse;
	SDL_Rect* camera;
public:
	void Create(Mouse* mouse, SDL_Rect* camera) {
		this->mouse = mouse;
		this->camera = camera;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;


		}		
	}

	virtual int isCollided(GameEntity* withThisEntity) {
		// Setup
		int result = -1;
		SDL_Rect entityBox = mouse->getCollisionBox(camera);

		if (withThisEntity->getName() == CLASS_TILE) {
			// Prepare variable
			Tile* tile = (Tile*)withThisEntity;			
			SDL_Rect tileBox = tile->getCollisionBox();
	/*		float topLeft = mouse->horizontalPosition + camera->x;
			float bottomLeft = mouse->verticalPosition + mouse->getSize()->h;*/
			// not check
			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_ROPE) {
				if (checkSquareCollision(entityBox, tileBox)) {
					SDL_Log("With rope");
					mouse->vy = -310;
				}
			}
			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE) {
				result = 0;
			}
			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE_JUMP) {
				if (
					(mouse->getCurrentStateType() != MouseSpriteState::STATE_PREJUMP 
						&& mouse->getCurrentStateType() != MouseSpriteState::STATE_JUMP)
					&& checkSquareCollision(entityBox, tileBox)
					/*&& ((tile->horizontalPosition > facePos - 10) &&
						(tile->horizontalPosition < facePos + 10) &&
						(tile->verticalPosition + tile->getSize()->h > mouse->verticalPosition + mouse->getSize()->h - 10) &&
						(tile->verticalPosition + tile->getSize()->h < mouse->verticalPosition + mouse->getSize()->h + 10))*/
					
					/*&& (bottomLeft + 2 >= tile->verticalPosition && bottomLeft + 2 - tile->verticalPosition <= TileSpriteState::TILE_HEIGHT)
					&& (topLeft >= tile->horizontalPosition && topLeft <= tile->horizontalPosition + tile->getSize()->w)*/
					) {
					SDL_Log("Collided on space");
					mouse->setCurrentStateType(MouseSpriteState::STATE_PREJUMP);
					result = 0;
				}
			}

			// Floor type collision
			if (
				tile->getCurrentStateType() == TileSpriteState::STATE_TILE_PATH
				|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_BASE
				|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_CORNER_LEFT
				|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_CORNER_RIGHT
			   ) {
				if (
					checkSquareCollision(entityBox, tileBox)
					) {
					mouse->isYCollidedWithMap = true;
					result = 1;
				}
			}

			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
				|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT) {
				if (
					checkSquareCollision(entityBox, tileBox)
					) {
					mouse->isXCollidedWithMap = true;
					result = 1;
				}
			}
		}
		return result;
	}

	bool checkSquareCollision(SDL_Rect a, SDL_Rect b) {
		// The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of rect A
		leftA = a.x;
		rightA = a.x + a.w;
		topA = a.y;
		bottomA = a.y + a.h;

		//Calculate the sides of rect B
		leftB = b.x;
		rightB = b.x + b.w;
		topB = b.y;
		bottomB = b.y + b.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}
};
