#pragma once
#include "SDL.h"
#include "player_collision_rule.h"
#include "mouse.h"
#include "cat.h"
#include "tile.h"
#include "tile_sprite_state.h"
#include <cmath>
#include "cat_behavior_component.h"
#include "rope.h"
#include "door.h"
#include "item.h"

class CatCollisionRule : public PlayerCollisionRule {
	Cat* gameEntity;
	SDL_Rect* camera;
	CatBehaviorComponent* behaviorComponent;
public:
	void Create(Cat* cat, SDL_Rect* camera, CatBehaviorComponent* behaviorComponent) {
		PlayerCollisionRule::Create(cat);
		this->gameEntity = cat;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_RAINBOW) {
			Rainbow* rainbow = (Rainbow*)m->getArg1();
			// hit with rainbow
			SDL_Log("Rainbow Hit111111111111!!");
			gameEntity->isStop = true;
			gameEntity->direction = rainbow->direction;
		}
		if (m->getArg1()->getName() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;
		}
		if (m->getArg1()->getName() == CLASS_ITEM) {
		}

		if (m->getArg1()->getName() == CLASS_ROPE) {
			// hit by the head
			if (gameEntity->vy < 0) {
				gameEntity->Receive(new Message(HEAD_HIT));
			}
			else {
				behaviorComponent->ChangeSpeedY(-characterBounceSpeedY);
			}
		}

		if (m->getArg1()->getName() == CLASS_DOOR) {
			
		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		// Collide with Rope
		if (withThisEntity->getName() == CLASS_ROPE) {
			Rope* rope = (Rope*)withThisEntity;
			bool isCollided = rope->getCollisionRule()->isCollided(rope, self, dt);
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_DOOR) {
			Door* door = (Door*)withThisEntity;
			bool isCollided = door->getCollisionRule()->isCollided(door, self, dt);
			if (isCollided) {
				bool isPowerDoor = DoorSpriteState::isPowerDoor(door->getCurrentStateType());
				//need to use dt
				if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_JUMP_BACK
					) {
					SDL_Log("JUMP AGAINST WALL");
					behaviorComponent->ChangeSpeedX(gameEntity->vx);
					gameEntity->direction = -gameEntity->direction;
					behaviorComponent->Move(dt*gameEntity->vx*gameEntity->direction, 0);
					gameEntity->setCurrentStateType(CatSpriteState::STATE_PREJUMP);
					behaviorComponent->jumpAgainstWall = true;
					behaviorComponent->resetStateIndicator = false;
				}
				else if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_PREJUMP) {

				}
				else if ((rand() % 2) + 1 == 2 && !isPowerDoor) {
					//SDL_Log("50 chance -- toggle!");
					gameEntity->Send(new Message(TOGGLE_DOOR, gameEntity));
				}
				else {
					behaviorComponent->Move(5 * -gameEntity->direction, 0);
					behaviorComponent->ChangeDirection();
				}

				if (isPowerDoor) {
					behaviorComponent->lastKnownRainbowDoor = door;
				}
			}
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_RAINBOW) {
			bool isCollided(checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox()));
			return isCollided;
		}

		return -1;
	}

	bool WhenCollideWithWall(Tile* tile, float dt) {
		if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_JUMP_BACK
			) {
			//SDL_Log("JUMP AGAINST WALL");

			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT) {
				behaviorComponent->ChangeSpeedX(gameEntity->vx);
				gameEntity->direction = GameEntity::RIGHT;
			}
			else {
				behaviorComponent->ChangeSpeedX(-gameEntity->vx);
				gameEntity->direction = GameEntity::LEFT;
			}
			behaviorComponent->Move(dt*gameEntity->vx, 0);
			gameEntity->setCurrentStateType(CatSpriteState::STATE_PREJUMP);
			behaviorComponent->jumpAgainstWall = true;
			behaviorComponent->resetStateIndicator = false;
		}
		else {
			behaviorComponent->ChangeDirection();
			behaviorComponent->Move(dt*(gameEntity->vx), 0);
		}

		return 1;
	}
	bool WhenHeadCollideWithCeil(Tile* tile, float dt) {
		if (gameEntity->vy < 0) {
			switch (gameEntity->getCurrentStateType()) {
			case CatSpriteState::STATE_INTHEAIR:
			case CatSpriteState::STATE_PRE_JUMP_BACK:
				//SDL_Log("HEAD");
				gameEntity->Receive(new Message(HEAD_HIT));
				gameEntity->setCurrentStateType(CatSpriteState::STATE_INTHEAIR);
				break;
			}
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheLeft(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::LEFT) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheRight(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::RIGHT) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenIsGoingToFallingDown(Tile* tile, float dt) {
		if (behaviorComponent->isOnTheGround(gameEntity->getCurrentStateType())) {
			//SDL_Log("Prejump");
			gameEntity->setCurrentStateType(CatSpriteState::STATE_PREJUMP);
		}

		return true;
	}
	bool WhenCollideWithFloor(Tile* tile, float dt) {
		gameEntity->isYCollidedWithMap = true;
		gameEntity->vy = 0;
		gameEntity->ay = 0;

		if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_INTHEAIR || gameEntity->getCurrentStateType() == CatSpriteState::STATE_JUMP_BACK) {
			//SDL_Log("From jump to stand");
			gameEntity->setCurrentStateType(CatSpriteState::STATE_STAND);
			//gameEntity->Send(new Message(MOUSE_JUMP_TO_STAND));
		}
		return 1;
	}

	GameEntity::Box GetPlayerBox() {
		return gameEntity->getCollisionBox();
	}

	int GetPlayerState() {
		return gameEntity->getCurrentStateType();
	}

	bool isUncollidableState(int state) {
		if (state == CatSpriteState::STATE_PREJUMP) {
			return true;
		}

		return false;
	}

	bool isPrejumpBack(int state) {
		if (state == CatSpriteState::STATE_PRE_JUMP_BACK) {
			return true;
		}

		return false;
	}

	//virtual int isCollidedWithMap(GameEntity* withThisEntity, float dt, std::vector<Tile*>* tileMap) {
	//	// Setup
	//	int result = -1;

	//	GameEntity::Box entityBox = gameEntity->getCollisionBox();
	//	int mouseCurrentState = gameEntity->getCurrentStateType();
	//	int mouseCenterX = entityBox.x + entityBox.w / 2;
	//	int mouseCenterY = entityBox.y + entityBox.h / 2;

	//	if (withThisEntity->getName() == CLASS_TILE) {
	//		// Prepare variable
	//		Tile* tile = (Tile*)withThisEntity;
	//		GameEntity::Box tileBox = tile->getCollisionBox();

	//		// index of this tile in the tilemap vector
	//		int pos = (int)tileBox.x / TileSpriteState::TILE_WIDTH + ((int)tileBox.y / TileSpriteState::TILE_HEIGHT) * TILE_ROWS;

	//		// Uncollidable
	//		if (mouseCurrentState == CatSpriteState::STATE_PREJUMP) {
	//			return 0;
	//		}

	//		//check against wall
	//		switch (tile->getCurrentStateType()) {
	//		case TileSpriteState::STATE_TILE_WALL_LEFT:
	//		case TileSpriteState::STATE_TILE_WALL_RIGHT:
	//			if (checkSquareCollision(entityBox, tileBox)) {
	//				if (gameEntity->getCurrentStateType() == CatSpriteState::STATE_JUMP_BACK
	//					) {
	//					SDL_Log("JUMP AGAINST WALL");

	//					if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT) {
	//						behaviorComponent->ChangeSpeedX(gameEntity->vx);
	//						gameEntity->direction = GameEntity::RIGHT;
	//					}
	//					else {
	//						behaviorComponent->ChangeSpeedX(-gameEntity->vx);
	//						gameEntity->direction = GameEntity::LEFT;
	//					}
	//					behaviorComponent->Move(dt*gameEntity->vx, 0);
	//					gameEntity->setCurrentStateType(CatSpriteState::STATE_PREJUMP);
	//					behaviorComponent->jumpAgainstWall = true;
	//					behaviorComponent->resetStateIndicator = false;
	//				}
	//				else {
	//					behaviorComponent->ChangeSpeedX(-gameEntity->vx);
	//					behaviorComponent->Move(dt*(gameEntity->vx), 0);
	//				}
	//			}
	//			break;
	//		}

	//		// need to check in detail to fix the "block position" that mouse can jump back to the floor ...
	//		if (mouseCurrentState == CatSpriteState::STATE_PRE_JUMP_BACK) {
	//			GameEntity::Box rightSide;
	//			GameEntity::Box leftSide;

	//			rightSide = gameEntity->getCollisionBox(camera);
	//			leftSide = gameEntity->getCollisionBox(camera);

	//			leftSide.x = leftSide.x - TileSpriteState::TILE_WIDTH; // -TileSpriteState::TILE_WIDTH / 2 - 1;
	//			rightSide.x = rightSide.x + rightSide.w + TileSpriteState::TILE_WIDTH; // +TileSpriteState::TILE_WIDTH / 2 + 1;

	//			bool isTileBelowIsSpace = false;

	//			if (pos + TILE_ROWS < tileMap->size()) {
	//				Tile* extraTile = tileMap->at(pos + TILE_ROWS);
	//				isTileBelowIsSpace = TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType();
	//			}

	//			// Check if left tile is a space.
	//			if ((leftSide.y + leftSide.h - 2 <= tileBox.y + tileBox.h && leftSide.y + leftSide.h >= tileBox.y + tileBox.h - 2)
	//				&& (leftSide.x + leftSide.w >= tileBox.x)
	//				&& (leftSide.x <= tileBox.x + tileBox.w)
	//				&& behaviorComponent->goingToJumpTo == GameEntity::LEFT
	//				) {

	//				if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE && (leftSide.x >= tileBox.x)) {
	//					if (((int)entityBox.y - TileSpriteState::TILE_HEIGHT) % TileSpriteState::TILE_HEIGHT < 5) {
	//						gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
	//					}
	//				}
	//				else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
	//					|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
	//					) {
	//					if (pos + 3 < tileMap->size()) {
	//						Tile* extraTile = tileMap->at(pos + 3);
	//						if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
	//							SDL_Log("LEFT WALL");
	//							gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
	//						}
	//					}

	//				}
	//			}

	//			if ((rightSide.y + rightSide.h - 2 <= tileBox.y + tileBox.h && rightSide.y + rightSide.h >= tileBox.y + tileBox.h - 2)
	//				&& (rightSide.x + rightSide.w >= tileBox.x)
	//				&& (rightSide.x <= tileBox.x + tileBox.w)
	//				&& behaviorComponent->goingToJumpTo == GameEntity::RIGHT
	//				) {
	//				if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE && (rightSide.x >= tileBox.x)) {
	//					if (((int)entityBox.y - TileSpriteState::TILE_HEIGHT) % TileSpriteState::TILE_HEIGHT < 5) {
	//						SDL_Log("Rightbox:: %f, %f", rightSide.x, rightSide.y);
	//						SDL_Log("RESOLVED:: %f, %f", tile->horizontalPosition, tile->verticalPosition);
	//						gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
	//					}
	//				}
	//				else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
	//					|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
	//					) {
	//					if (pos - 3 < tileMap->size()) {
	//						Tile* extraTile = tileMap->at(pos - 3);
	//						if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
	//							SDL_Log("RIGHT WALL");
	//							gameEntity->setCurrentStateType(CatSpriteState::STATE_JUMP_BACK);
	//						}
	//					}

	//				}
	//			}
	//		}

	//		// check head on the floor to bounce back when jump
	//		switch (gameEntity->getCurrentStateType()) {
	//			case CatSpriteState::STATE_INTHEAIR:
	//			case CatSpriteState::STATE_PRE_JUMP_BACK:
	//			if (
	//				(entityBox.x + 5 <= tileBox.x + tileBox.w && entityBox.x + entityBox.w - 5 >= tileBox.x)
	//				&& (entityBox.y <= tileBox.y + tileBox.h / 2) && (entityBox.y + 1 >= tileBox.y + tileBox.h / 2 - 1)
	//				&& (TileSpriteState::isFloor(tile->getCurrentStateType()))
	//				&& gameEntity->vy < 0
	//				) {
	//				gameEntity->Receive(new Message(HEAD_HIT));
	//				gameEntity->setCurrentStateType(CatSpriteState::STATE_INTHEAIR);
	//			}
	//			break;
	//		}

	//		// check the edge of the floor (collide by x-center)
	//		if (
	//			(mouseCenterX - 1 <= tileBox.x + tileBox.w && mouseCenterX + 1 >= tileBox.x)
	//			&& (entityBox.y + entityBox.h - 1 <= tileBox.y + 1) && (entityBox.y + entityBox.h + 1 >= tileBox.y)
	//			)
	//		{
	//			switch (tile->getCurrentStateType()) {
	//				// if there is nothing under your foot! -> should jump and falling down
	//			case TileSpriteState::STATE_TILE_SPACE:
	//			case TileSpriteState::STATE_TILE_ROPE:
	//				if (behaviorComponent->isOnTheGround(mouseCurrentState)) {
	//					SDL_Log("Prejump");
	//					gameEntity->setCurrentStateType(CatSpriteState::STATE_PREJUMP);
	//				}
	//				break;
	//			}
	//		}

	//		// check if its foot collision (for the floor)
	//		if (
	//			(entityBox.x + 5 <= tileBox.x + tileBox.w && entityBox.x + entityBox.w - 5 >= tileBox.x)
	//			&& (entityBox.y + entityBox.h - 1 <= tileBox.y + 1) && (entityBox.y + entityBox.h + 1 >= tileBox.y)
	//			)
	//		{
	//			switch (tile->getCurrentStateType()) {
	//				// collide with the floor
	//			case TileSpriteState::STATE_TILE_PATH:
	//			case TileSpriteState::STATE_TILE_BASE:
	//			case TileSpriteState::STATE_TILE_CORNER_LEFT:
	//			case TileSpriteState::STATE_TILE_CORNER_RIGHT:
	//				gameEntity->isYCollidedWithMap = true;
	//				gameEntity->vy = 0;
	//				gameEntity->ay = 0;
	//				if (mouseCurrentState == CatSpriteState::STATE_INTHEAIR || mouseCurrentState == CatSpriteState::STATE_JUMP_BACK) {
	//					SDL_Log("From jump to stand");
	//					gameEntity->setCurrentStateType(CatSpriteState::STATE_STAND);
	//				}

	//				result = 1;
	//				break;
	//			}
	//		}




	//	}
	//	return result;
	//}



};
