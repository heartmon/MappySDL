#pragma once
#include "component.h"
#include "message.h"
#include "global_constant.h"

class ItemBehaviorComponent : public Component {
	int ropeLife;
	ItemSpriteState* spriteState;

	float ropeJumpInterval = .2f;
	float ropeJumpTime = 0;

public:
	virtual void Create(AvancezLib* system, GameEntity* go, std::vector<GameEntity*> * gameObjects, RopeSpriteState* spriteState) {
		Component::Create(system, go, gameObjects);

		this->spriteState = spriteState;
	}
	virtual void Init() {
		ropeLife = 4;
		updateSpriteWithCurrentLife();
	}

	int getRopeLive() {
		return ropeLife;
	}

	void Update(float dt) {
		if (gameEntity->getCurrentStateType() == ItemSpriteState::STATE_ROPE_JUMP_ON) {
			if (ropeJumpTime > ropeJumpInterval || gameEntity->animationFrame >= ItemSpriteState::FRAME_ROPE_JUMP_ON) {
				//stop jump
				gameEntity->setCurrentStateType(ItemSpriteState::STATE_ROPE_STATIC);
				ropeJumpTime = 0;
			}

			ropeJumpTime += dt;
		}
	}

	void updateSpriteWithCurrentLife() {
		switch (ropeLife) {
		case 4:
			//green
			spriteState->getSpriteSheet()->SetTintColor(0, 255, 0);
			break;
		case 3:
			//blue
			spriteState->getSpriteSheet()->SetTintColor(0, 0, 255);
			break;
		case 2:
			//white
			spriteState->getSpriteSheet()->SetTintColor(255, 255, 255);
			break;
		case 1:
			//red
			spriteState->getSpriteSheet()->SetTintColor(255, 0, 0);
			break;
		default:

			break;
		}
	}

	void restoreLife() {
		ropeLife = 4;
		updateSpriteWithCurrentLife();
	}

	void ropeSeparated() {
		gameEntity->setCurrentStateType(ItemSpriteState::STATE_ROPE_SEPARATE);
	}

	void removeLife() {
		ropeLife -= 1;
		updateSpriteWithCurrentLife();
		if (ropeLife <= 0) {
			gameEntity->setCurrentStateType(ItemSpriteState::STATE_ROPE_SEPARATE);
		}
		else {
			gameEntity->setCurrentStateType(ItemSpriteState::STATE_ROPE_JUMP_ON);
		}
	}

	virtual void Receive(Message msg) {
		// if HIT by other game entities
		if (msg.getMessageType() == HIT) {
			// hit by mouse
			if (msg.getArg1() == CLASS_MOUSE) {
				SDL_Log("HIT ROPE!");
			}
		}
	}

	ItemSpriteState* getSpriteState() {
		return spriteState;
	}
	//void Destroy() {

	//}
private:

};