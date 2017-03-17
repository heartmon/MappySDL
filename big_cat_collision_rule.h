#pragma once
#include "SDL.h"
#include "player_collision_rule.h"
#include "mouse.h"
#include "big_cat.h"
#include "tile.h"
#include "tile_sprite_state.h"
#include <cmath>
#include "big_cat_behavior_component.h"
#include "rope.h"
#include "door.h"
#include "item.h"
#include "rainbow.h"

class BigCatCollisionRule : public PlayerCollisionRule {
	BigCat* gameEntity;
	SDL_Rect* camera;
	BigCatBehaviorComponent* behaviorComponent;
public:
	void Create(BigCat* cat, SDL_Rect* camera, BigCatBehaviorComponent* behaviorComponent) {
		PlayerCollisionRule::Create(cat);
		this->gameEntity = cat;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}
	virtual void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_RAINBOW) {
			Rainbow* rainbow = (Rainbow*)m->getArg1();
			// hit with rainbow
			gameEntity->isStop = true;
			gameEntity->direction = rainbow->direction;
		}
		if (m->getArg1()->getName() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;
		}
		if (m->getArg1()->getName() == CLASS_ITEM) {
			behaviorComponent->ThinkToPossessItem((Item*)m->getArg1());
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
			bool isPowerDoor = DoorSpriteState::isPowerDoor(m->getArg1()->getCurrentStateType());
			if ((rand() % 2) + 1 == 2 && !isPowerDoor) {
				//SDL_Log("50 chance -- toggle!");
				gameEntity->Send(new Message(TOGGLE_DOOR, gameEntity));
			}
			else {
				behaviorComponent->Move(5 * -gameEntity->direction, 0);
				behaviorComponent->ChangeDirection();
			}
			if (isPowerDoor) {
				behaviorComponent->lastKnownRainbowDoor = (Door*)m->getArg1();
			}
		}

		if (m->getArg1()->getName() == CLASS_MOUSE) {
			switch (gameEntity->getCurrentStateType()) {
				case BigCatSpriteState::STATE_IN_ITEM:
					SDL_Log("Mouse will get bonus score");
					behaviorComponent->ShowScore();
					gameEntity->Send(new Message(UPDATE_SCORE, gameEntity));
					break;
			}
		}
	}

	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		// Collide with Rope
		if (withThisEntity->getName() == CLASS_ROPE) {
			Rope* rope = (Rope*)withThisEntity;
			bool isCollided = rope->getCollisionRule()->isCollided(rope, self, dt);
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_DOOR && behaviorComponent->isOnTheGround(gameEntity->getCurrentStateType())) {
			Door* door = (Door*)withThisEntity;
			bool isCollided = door->getCollisionRule()->isCollided(door, self, dt);
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_RAINBOW) {
			bool isCollided(checkSquareCollision(self->getCollisionBox(), withThisEntity->getCollisionBox()));
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_ITEM) {
			GameEntity::Box bcBox = self->getCollisionBox();
			GameEntity::Box itemBox = withThisEntity->getCollisionBox();
			bcBox.x = bcBox.x + bcBox.w / 2 - 1;
			bcBox.w = 2;
			itemBox.x = itemBox.x + itemBox.w / 2 - 1;
			itemBox.w = 2;

			bool isCollided(checkSquareCollision(bcBox, itemBox));
			return isCollided;
		}

		return -1;
	}

	bool WhenCollideWithWall(Tile* tile, float dt) {
		if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_JUMP_BACK
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
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_PREJUMP);
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
			case BigCatSpriteState::STATE_INTHEAIR:
			case BigCatSpriteState::STATE_PRE_JUMP_BACK:
				//SDL_Log("HEAD");
				gameEntity->Receive(new Message(HEAD_HIT));
				gameEntity->setCurrentStateType(BigCatSpriteState::STATE_INTHEAIR);
				break;
			}
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheLeft(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::LEFT) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheRight(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::RIGHT) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenIsGoingToFallingDown(Tile* tile, float dt) {
		if (behaviorComponent->isOnTheGround(gameEntity->getCurrentStateType())) {
			//SDL_Log("Prejump");
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_PREJUMP);
		}

		return true;
	}
	bool WhenCollideWithFloor(Tile* tile, float dt) {
		gameEntity->isYCollidedWithMap = true;
		gameEntity->vy = 0;
		gameEntity->ay = 0;

		if (gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_INTHEAIR || gameEntity->getCurrentStateType() == BigCatSpriteState::STATE_JUMP_BACK) {
			//SDL_Log("From jump to stand");
			gameEntity->setCurrentStateType(BigCatSpriteState::STATE_STAND);
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
		if (state == BigCatSpriteState::STATE_PREJUMP) {
			return true;
		}

		return false;
	}

	bool isPrejumpBack(int state) {
		if (state == BigCatSpriteState::STATE_PRE_JUMP_BACK) {
			return true;
		}

		return false;
	}
};
