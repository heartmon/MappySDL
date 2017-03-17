#pragma once
#include "SDL.h"
#include "player_collision_rule.h"
#include "mouse.h"
#include "tile.h"
#include "tile_sprite_state.h"
#include <cmath>
#include "mouse_behavior_component.h"
#include "rope.h"
#include "door.h"
#include "item.h"
#include "cat_sprite_state.h"
#include "big_cat_sprite_state.h"

class MouseCollisionRule : virtual public PlayerCollisionRule {
	Mouse* gameEntity;
	SDL_Rect* camera;
	MouseBehaviorComponent* behaviorComponent;

public:
	void Create(Mouse* mouse, SDL_Rect* camera, MouseBehaviorComponent* behaviorComponent) {
		PlayerCollisionRule::Create(mouse);
		this->gameEntity = mouse;
		this->camera = camera;
		this->behaviorComponent = behaviorComponent;
	}
	void didHit(Message* m) {
		if (m->getArg1()->getName() == CLASS_CAT) {
			SDL_Log("Dead by cat");
			gameEntity->Receive(new Message(MOUSE_DIE, gameEntity));
		}
		if (m->getArg1()->getName() == CLASS_BIG_CAT) {
			switch (m->getArg1()->getCurrentStateType()) {
				case BigCatSpriteState::STATE_WALK:
					SDL_Log("Dead by big cat");
					gameEntity->Receive(new Message(MOUSE_DIE, gameEntity));
					break;
			}
		}
		if (m->getArg1()->getName() == CLASS_TILE) {
			//mouse->isCollidedWithMap = true;
		}		
		if (m->getArg1()->getName() == CLASS_ITEM) {
		}

		if (m->getArg1()->getName() == CLASS_ROPE) {
			SDL_Log("With rope");

			// hit by the head
			if (gameEntity->vy < 0) {
				gameEntity->Receive(new Message(HEAD_HIT));
			}
			else {
				GameEntity::Box mouseBox = gameEntity->getCollisionBox(camera);
				int yPos = ceil(mouseBox.y / TileSpriteState::TILE_HEIGHT);
				//SDL_Log("Current speed Y %f, Y pos: %d", gameEntity->vy, yPos);
				behaviorComponent->ChangeSpeedY(-characterBounceSpeedY);
				gameEntity->Send(new Message(MOUSE_JUMP_ON_ROPE));
			}
			//mouse->vy = -430;
		}

		if (m->getArg1()->getName() == CLASS_DOOR) {
			//behaviorComponent->ChangeSpeedX(0);
			//behaviorComponent->Move(0.03 * gameEntity->vx * -gameEntity->direction, 0);
		}
	}

	int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) {
		// Collide with Rope
		if (withThisEntity->getName() == CLASS_ROPE) {
			Rope* rope = (Rope*)withThisEntity;
			bool isCollided = rope->getCollisionRule()->isCollided(rope, self, dt);
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_ITEM) {
			Item* item = (Item*)withThisEntity;
			bool isCollided = item->getCollisionRule()->isCollided(item, self, dt);
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_DOOR && behaviorComponent->isOnTheGround(gameEntity->getCurrentStateType())) {
			Door* door = (Door*)withThisEntity;
			bool isCollided = door->getCollisionRule()->isCollided(door, self, dt);
			if (isCollided) {
				//need to use dt
				behaviorComponent->Move(dt * gameEntity->vx * -gameEntity->direction, 0);
			}
			return isCollided;
		}
		if (withThisEntity->getName() == CLASS_CAT) {
			GameEntity::Box mouseBox = self->getCollisionBox(camera);
			mouseBox.x = mouseBox.x + mouseBox.w / 2 - 5;
			mouseBox.w = 10;
			bool isCollided = checkSquareCollision(mouseBox, withThisEntity->getCollisionBox());
			bool isCatCollidable =
				withThisEntity->getCurrentStateType() == CatSpriteState::STATE_WALK
				;
			bool isSelfCollidable =
				self->getCurrentStateType() == MouseSpriteState::STATE_STAND
				|| self->getCurrentStateType() == MouseSpriteState::STATE_WALK;

			return isCollided && isCatCollidable && isSelfCollidable;
		}

		if (withThisEntity->getName() == CLASS_BIG_CAT) {
			GameEntity::Box mouseBox = self->getCollisionBox(camera);
			mouseBox.x = mouseBox.x + mouseBox.w / 2 - 5;
			mouseBox.w = 10;
			bool isCollided = checkSquareCollision(mouseBox, withThisEntity->getCollisionBox());
			bool isSelfCollidable =
				self->getCurrentStateType() == MouseSpriteState::STATE_STAND
				|| self->getCurrentStateType() == MouseSpriteState::STATE_WALK;
			return isCollided && isSelfCollidable;
		}

		return -1;
	}

	bool WhenCollideWithWall(Tile* tile, float dt) {
		if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK
			) {
			SDL_Log("JUMP AGAINST WALL");

			if (tile->getCurrentStateType() == TileSpriteState::STATE_TILE_WALL_RIGHT) {
				behaviorComponent->ChangeSpeedX(gameEntity->vx);
				gameEntity->direction = GameEntity::RIGHT;
			}
			else {
				behaviorComponent->ChangeSpeedX(-gameEntity->vx);
				gameEntity->direction = GameEntity::LEFT;
			}
			behaviorComponent->Move(dt*gameEntity->vx, 0);
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_PREJUMP);
			behaviorComponent->jumpAgainstWall = true;
			behaviorComponent->resetStateIndicator = false;
		}
		else {
			behaviorComponent->ChangeSpeedX(-gameEntity->vx);
			behaviorComponent->Move(dt*(gameEntity->vx), 0);
		}

		return 1;
	}
	bool WhenHeadCollideWithCeil(Tile* tile, float dt) {
		if (gameEntity->vy < 0) {
			switch (gameEntity->getCurrentStateType()) {
			case MouseSpriteState::STATE_INTHEAIR:
			case MouseSpriteState::STATE_PRE_JUMP_BACK:
				SDL_Log("HEAD");
				//behaviorComponent->ChangeSpeedY(-mouse->vy);
				gameEntity->Receive(new Message(HEAD_HIT));
				gameEntity->setCurrentStateType(MouseSpriteState::STATE_INTHEAIR);
				break;
			}
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheLeft(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::LEFT) {
			gameEntity->direction = GameEntity::LEFT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenReadyToJumpbackToTheRight(Tile* tile, float dt) {
		if (behaviorComponent->goingToJumpTo == GameEntity::RIGHT) {
			gameEntity->direction = GameEntity::RIGHT;
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_JUMP_BACK);
		}

		return true;
	}
	bool WhenIsGoingToFallingDown(Tile* tile, float dt) {
		if (behaviorComponent->isOnTheGround(gameEntity->getCurrentStateType())) {
			SDL_Log("Prejump");
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_PREJUMP);
		}

		return true;
	}
	bool WhenCollideWithFloor(Tile* tile, float dt) {
		gameEntity->isYCollidedWithMap = true;
		gameEntity->vy = 0;
		gameEntity->ay = 0;
		//behaviorComponent->Move(0, -dt*mouse->vy);
		if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_INTHEAIR) {
			if (behaviorComponent->isGoingToDie) {
				gameEntity->Receive(new Message(MOUSE_DIE, gameEntity));
				return 1;
			}
		}

		if (gameEntity->getCurrentStateType() == MouseSpriteState::STATE_INTHEAIR || gameEntity->getCurrentStateType() == MouseSpriteState::STATE_JUMP_BACK) {
			SDL_Log("From jump to stand");
			gameEntity->setCurrentStateType(MouseSpriteState::STATE_STAND);
			gameEntity->Send(new Message(MOUSE_JUMP_TO_STAND));
		}
		return 1;
	}

	GameEntity::Box GetPlayerBox() {
		return gameEntity->getCollisionBox(camera);
	}

	int GetPlayerState() {
		return gameEntity->getCurrentStateType();
	}

	bool isUncollidableState(int state) {
		if (state == MouseSpriteState::STATE_PREJUMP) {
			return true; 
		}

		return false;
	}

	bool isPrejumpBack(int state) {
		if (state == MouseSpriteState::STATE_PRE_JUMP_BACK) {
			return true;
		}

		return false;
	}
	
};
