#pragma once
#include "collision_rule_interface.h"
#include "tile.h"
#include "tile_sprite_state.h"

class PlayerCollisionRule : public CollisionRuleInterface {

public:
	virtual bool WhenCollideWithWall(Tile* tile, float dt) = 0;
	virtual bool WhenHeadCollideWithCeil(Tile* tile, float dt) = 0;
	virtual bool WhenReadyToJumpbackToTheLeft(Tile* tile, float dt) = 0;
	virtual bool WhenReadyToJumpbackToTheRight(Tile* tile, float dt) = 0;
	virtual bool WhenIsGoingToFallingDown(Tile* tile, float dt) = 0;
	virtual bool WhenCollideWithFloor(Tile* tile, float dt) = 0;
	
	virtual GameEntity::Box GetPlayerBox() = 0;
	virtual int GetPlayerState() = 0;

	virtual bool isUncollidableState(int state) = 0;
	virtual bool isPrejumpBack(int state) = 0;


	virtual void Create(GameEntity* gameEntity) {
		this->gameEntity = gameEntity;
	}

	virtual int isCollidedWithMap(GameEntity* withThisEntity, float dt, ObjectPool<Tile>* tileMap) {
		// Setup
		int result = -1;

		GameEntity::Box playerBox = GetPlayerBox();
		int playerCurrentState = GetPlayerState();

		int playerCenterX = playerBox.x + playerBox.w / 2;
		int playerCenterY = playerBox.y + playerBox.h / 2;

		if (withThisEntity->getName() == CLASS_TILE) {
			// Prepare variable
			Tile* tile = (Tile*)withThisEntity;
			GameEntity::Box tileBox = tile->getCollisionBox();

			// index of this tile in the tilemap vector
			int pos = (int)tileBox.x / TileSpriteState::TILE_WIDTH + ((int)tileBox.y / TileSpriteState::TILE_HEIGHT) * TILE_ROWS;

			// Uncollidable
			if (isUncollidableState(playerCurrentState)) {
				return 0;
			}

			//check against wall
			switch (tile->getCurrentStateType()) {
			case TileSpriteState::STATE_TILE_WALL_LEFT:
			case TileSpriteState::STATE_TILE_WALL_RIGHT:
				if (checkSquareCollision(playerBox, tileBox)) {
					result = WhenCollideWithWall(tile, dt);
				}
				break;
			}

			// need to check in detail to fix the "block position" that mouse can jump back to the floor ...
			if (isPrejumpBack(playerCurrentState)) {
				GameEntity::Box rightSide;
				GameEntity::Box leftSide;

				rightSide = GetPlayerBox();
				leftSide = GetPlayerBox();

				leftSide.x = leftSide.x - TileSpriteState::TILE_WIDTH; // -TileSpriteState::TILE_WIDTH / 2 - 1;
				rightSide.x = rightSide.x + rightSide.w + TileSpriteState::TILE_WIDTH; // +TileSpriteState::TILE_WIDTH / 2 + 1;

				bool isTileBelowIsSpace = false;

				if ((leftSide.y + leftSide.h - 2 <= tileBox.y + tileBox.h + 5 && leftSide.y + leftSide.h >= tileBox.y + tileBox.h - 2)
					&& (leftSide.x + leftSide.w >= tileBox.x)
					&& (leftSide.x <= tileBox.x + tileBox.w)
					) {

					if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE && (leftSide.x >= tileBox.x)) {
						//if (((int)GetPlayerBox().y - TileSpriteState::TILE_HEIGHT) % TileSpriteState::TILE_HEIGHT < 40) {
							result = WhenReadyToJumpbackToTheLeft(tile, dt);
						//}
					}
					else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
						|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
						) {
						if (pos + 3 < tileMap->pool.size()) {
							Tile* extraTile = SearchTileMapByPosition(tileMap, pos + 3);
							if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
								SDL_Log("LEFT WALL");
								result = WhenReadyToJumpbackToTheLeft(tile, dt);
							}
						}

					}
				}

				if ((rightSide.y + rightSide.h - 2 <= tileBox.y + tileBox.h + 5 && rightSide.y + rightSide.h >= tileBox.y + tileBox.h - 2)
					&& (rightSide.x + rightSide.w >= tileBox.x)
					&& (rightSide.x <= tileBox.x + tileBox.w)
					
					) {
					//&& behaviorComponent->goingToJumpTo == GameEntity::RIGHT
					if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_SPACE && (rightSide.x >= tileBox.x)) {
						//if (((int)GetPlayerBox().y - TileSpriteState::TILE_HEIGHT) % TileSpriteState::TILE_HEIGHT < 40) {
							result = WhenReadyToJumpbackToTheRight(tile, dt);
						//}
					}
					else if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_LEFT
						|| tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT
						) {
						if (pos - 3 < tileMap->pool.size()) {
							Tile* extraTile = SearchTileMapByPosition(tileMap, pos - 3);
							if (TileSpriteState::STATE_TILE_SPACE == extraTile->getCurrentStateType()) {
								result = WhenReadyToJumpbackToTheRight(tile, dt);
							}
						}

					}
				}
			}

			// check head on the floor to bounce back when jump
			
			if (
				(playerBox.x + 5 <= tileBox.x + tileBox.w && playerBox.x + playerBox.w - 5 >= tileBox.x)
				&& (playerBox.y <= tileBox.y + tileBox.h / 2) && (playerBox.y + 1 >= tileBox.y + tileBox.h / 2 - 1)
				&& (TileSpriteState::isFloor(tile->getCurrentStateType()))
				&& gameEntity->vy < 0
				) {
				result = WhenHeadCollideWithCeil(tile, dt);
			}
					


			// check the edge of the floor (collide by x-center)
			if (
				(playerCenterX - 1 <= tileBox.x + tileBox.w && playerCenterX + 1 >= tileBox.x)
				&& (playerBox.y + playerBox.h - 1 <= tileBox.y + 1) && (playerBox.y + playerBox.h + 1 >= tileBox.y)
				)
			{
				switch (tile->getCurrentStateType()) {
					// if there is nothing under your foot! -> should jump and falling down
					case TileSpriteState::STATE_TILE_SPACE:
					case TileSpriteState::STATE_TILE_ROPE:
						result = WhenIsGoingToFallingDown(tile, dt);
						break;
				}
			}

			// check if its foot collision (for the floor)
			if (
				(playerBox.x + 5 <= tileBox.x + tileBox.w && playerBox.x + playerBox.w - 5 >= tileBox.x)
				&& (playerBox.y + playerBox.h - 1 <= tileBox.y + 1) && (playerBox.y + playerBox.h + 1 >= tileBox.y)
				)
			{
				switch (tile->getCurrentStateType()) {
					// collide with the floor
				case TileSpriteState::STATE_TILE_PATH:
				case TileSpriteState::STATE_TILE_BASE:
				case TileSpriteState::STATE_TILE_CORNER_LEFT:
				case TileSpriteState::STATE_TILE_CORNER_RIGHT:
					result = WhenCollideWithFloor(tile, dt);
				}
			}

		}
		return result;
	}

	Tile* SearchTileMapByPosition(ObjectPool<Tile>* tileMapPool, int search) {
		for (auto it = tileMapPool->pool.begin(); it != tileMapPool->pool.end(); it++)
		{
			if (search == (*it)->getId()) {
				return (*it);
			}
		}

		return NULL;
	}

};