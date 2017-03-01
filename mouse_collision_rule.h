#pragma once
#include "SDL.h"
#include "collision_rule_interface.h"
#include "mouse.h"
#include "tile.h"
#include "tile_sprite_state.h"
#include <cmath>
#include "mouse_behavior_component.h"
#include "rope.h"

class MouseCollisionRule : public CollisionRuleInterface {
	Mouse* mouse;
	SDL_Rect* camera;
	MouseBehaviorComponent* behaviorComponent;
public:
	void Create(Mouse* mouse, SDL_Rect* camera, MouseBehaviorComponent* behaviorComponent) {
		this->mouse = mouse;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;


		}		

		if (m->getArg1() == CLASS_ROPE) {
			SDL_Log("With rope");
			behaviorComponent->ChangeSpeedY(-430);
			//mouse->vy = -430;
		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		// Collide with Rope
		if (withThisEntity->getName() == CLASS_ROPE) {
			Rope* rope = (Rope*)withThisEntity;
			bool isCollided = rope->getCollisionRule()->isCollided(rope, self, dt);
			return isCollided;
		}

		return -1;
	}

	virtual int isCollidedWithMap(GameEntity* withThisEntity, float dt, std::vector<Tile*>* tileMap) {
		// Setup
		int result = -1;

		GameEntity::Box entityBox = mouse->getCollisionBox(camera);
		int mouseCurrentState = mouse->getCurrentStateType();
		int mouseCenterX = entityBox.x + entityBox.w / 2;
		int mouseCenterY = entityBox.y + entityBox.h / 2;

		if (withThisEntity->getName() == CLASS_TILE) {
			// Prepare variable
			Tile* tile = (Tile*)withThisEntity;			
			GameEntity::Box tileBox = tile->getCollisionBox();

			// index of this tile in the tilemap vector
			int pos = (int)tileBox.x / TileSpriteState::TILE_WIDTH + ((int)tileBox.y / TileSpriteState::TILE_HEIGHT) * TILE_ROWS;

			// Uncollidable
			if (mouseCurrentState == MouseSpriteState::STATE_PREJUMP) {
				return 0;
			}

			// Start checking for each tile type at each game entity (mouse) state
			/*if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_ROPE) {
				if (checkSquareCollision(entityBox, tileBox)) {
					SDL_Log("With rope");
					mouse->vy = -430;
				}
			}*/

			//check against wall
			switch (tile->getCurrentStateType()) {
				case TileSpriteState::STATE_TILE_WALL_LEFT:
				case TileSpriteState::STATE_TILE_WALL_RIGHT:
					if (checkSquareCollision(entityBox, tileBox)) {
						if (mouse->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK
							) {
							SDL_Log("JUMP AGAINST WALL");

							if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT) {
								behaviorComponent->ChangeSpeedX(mouse->vx);
								mouse->direction = GameEntity::RIGHT;
							}
							else {
								behaviorComponent->ChangeSpeedX(-mouse->vx);
								mouse->direction = GameEntity::LEFT;
							}
							behaviorComponent->Move(dt*mouse->vx, 0);
							mouse->setCurrentStateType(MouseSpriteState::STATE_PREJUMP);
							behaviorComponent->jumpAgainstWall = true;
							behaviorComponent->resetStateIndicator = false;
						}
						else {
							behaviorComponent->ChangeSpeedX(-mouse->vx);
							behaviorComponent->Move(dt*(mouse->vx), 0);
						}
					}
				break;
			}

			// need to check in detail to fix the "block position" that mouse can jump back to the floor ...
			if (mouseCurrentState == MouseSpriteState::STATE_PRE_JUMP_BACK) {
				GameEntity::Box rightSide;
				GameEntity::Box leftSide;

				rightSide = mouse->getCollisionBox(camera);
				leftSide = mouse->getCollisionBox(camera);

				leftSide.x = leftSide.x - TileSpriteState::TILE_WIDTH;
				rightSide.x = rightSide.x + rightSide.w + TileSpriteState::TILE_WIDTH;

				bool isTileBelowIsSpace = false;

				if (pos + TILE_ROWS < tileMap->size()) {
					Tile* extraTile = tileMap->at(pos + TILE_ROWS);
					isTileBelowIsSpace = TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType();
				}

				if ((leftSide.y - 2 <= tileBox.y + 2 && leftSide.y + 2 >= tileBox.y)
					&& (leftSide.x + leftSide.w + 1 >= tileBox.x)
					&& (leftSide.x <= tileBox.x + tileBox.w)
					&& behaviorComponent->goingToJumpTo == GameEntity::LEFT
					) {
					
					if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE) {
						if (!isTileBelowIsSpace) {
							SDL_Log("LEFT");
							mouse->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
						}
					}
					else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
						|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
						) {
						if (pos + 3 < tileMap->size()) {
							Tile* extraTile = tileMap->at(pos + 3);
							if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
								SDL_Log("LEFT WALL");
								mouse->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
							}
						}
						
					}
				}

				if ((rightSide.y - 2 <= tileBox.y + 2 && rightSide.y + 2 >= tileBox.y)
					&& (rightSide.x + rightSide.w + 1 >= tileBox.x)
					&& (rightSide.x <= tileBox.x + tileBox.w)
					&& behaviorComponent->goingToJumpTo == GameEntity::RIGHT
					) {
					if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE) {
						if (!isTileBelowIsSpace) {
							SDL_Log("RIGHT");
							mouse->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
						}
					}
					else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
						|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
						) {
						if (pos - 3 < tileMap->size()) {
							Tile* extraTile = tileMap->at(pos - 3);
							if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
								SDL_Log("RIGHT WALL");
								mouse->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
							}
						}

					}
				}
			}

			// check head on the floor to bounce back when jump
			switch (mouse->getCurrentStateType()) {
				case MouseSpriteState::STATE_INTHEAIR:
				case MouseSpriteState::STATE_PRE_JUMP_BACK:
					if (
						(entityBox.x + 5 <= tileBox.x + tileBox.w && entityBox.x + entityBox.w - 5 >= tileBox.x)
						&& (entityBox.y <= tileBox.y + tileBox.h/2) && (entityBox.y + 1 >= tileBox.y + tileBox.h/2 - 1)
						&& TileSpriteState::isFloor(tile->getCurrentStateType())
						&& mouse->vy < 0
						) {
				
							SDL_Log("HEAD");
							behaviorComponent->ChangeSpeedY(-mouse->vy);
							mouse->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
					}
				break;
			}

			// check the edge of the floor (collide by x-center)
			if (
				(mouseCenterX - 1 <= tileBox.x + tileBox.w && mouseCenterX + 1 >= tileBox.x)
				&& (entityBox.y + entityBox.h - 1 <= tileBox.y + 1) && (entityBox.y + entityBox.h + 1 >= tileBox.y)
				)
			{
				switch (tile->getCurrentStateType()) {
					// if there is nothing under your foot! -> should jump and falling down
				case TileSpriteState::STATE_TILE_SPACE:
				case TileSpriteState::STATE_TILE_ROPE:
					if (behaviorComponent->isOnTheGround(mouseCurrentState)) {
						SDL_Log("Prejump");
						mouse->setCurrentStateType(MouseSpriteState::STATE_PREJUMP);
					}
					break;
				}
			}

			// check if its foot collision (for the floor)
			if (
				(entityBox.x + 5 <= tileBox.x + tileBox.w && entityBox.x + entityBox.w - 5 >= tileBox.x)
				&& (entityBox.y + entityBox.h - 1 <= tileBox.y + 1) && (entityBox.y + entityBox.h + 1 >= tileBox.y)
				)
			{
				switch (tile->getCurrentStateType()) {
					// collide with the floor
					case TileSpriteState::STATE_TILE_PATH:
					case TileSpriteState::STATE_TILE_BASE:
					case TileSpriteState::STATE_TILE_CORNER_LEFT:
					case TileSpriteState::STATE_TILE_CORNER_RIGHT:
						mouse->isYCollidedWithMap = true;
						mouse->vy = 0;
						mouse->ay = 0;
						//behaviorComponent->Move(0, -dt*mouse->vy);
						if (mouseCurrentState == MouseSpriteState::STATE_INTHEAIR || mouseCurrentState == MouseSpriteState::STATE_JUMP_BACK) {
							SDL_Log("From jump to stand");
							mouse->setCurrentStateType(MouseSpriteState::STATE_STAND);
							mouse->Send(new Message(MOUSE_JUMP_TO_STAND));
						}
						result = 1;
						break;
				}
			}

			


		}
		return result;
	}


	
};
