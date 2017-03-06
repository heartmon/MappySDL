#pragma once
#include "component.h"
#include "message.h"
#include "global_constant.h"

class RopeBehaviorComponent : public Component {
	int ropeLife;
	RopeSpriteState* spriteState;

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

	virtual void RoundInit() {
		ropeLife = 4;
		gameEntity->setCurrentStateType(RopeSpriteState::STATE_ROPE_STATIC);
		updateSpriteWithCurrentLife();
	}

	int getRopeLive() {
		return ropeLife;
	}

	void Update(float dt) {
		if (gameEntity->getCurrentStateType() == RopeSpriteState::STATE_ROPE_JUMP_ON) {
			if (ropeJumpTime > ropeJumpInterval || gameEntity->animationFrame >= RopeSpriteState::FRAME_ROPE_JUMP_ON) {
				//stop jump
				gameEntity->setCurrentStateType(RopeSpriteState::STATE_ROPE_STATIC);
				ropeJumpTime = 0;
			}

			ropeJumpTime += dt;
		}
	}

	void updateSpriteWithCurrentLife() {
		switch (ropeLife) {
		case 4:
			//green
			spriteState->getSpriteSheet()->SetTintColor(0,255,0);
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
			spriteState->getSpriteSheet()->SetTintColor(255, 0, 0);
			break;
		}
	}

	void restoreLife() {
		if (gameEntity->getCurrentStateType() != RopeSpriteState::STATE_ROPE_SEPARATE) {
			ropeLife = 4;
			updateSpriteWithCurrentLife();
		}
	}

	void ropeSeparated() {
		gameEntity->setCurrentStateType(RopeSpriteState::STATE_ROPE_SEPARATE);
	}

	void removeLife() {
		ropeLife -= 1;
		updateSpriteWithCurrentLife();
		if (ropeLife <= 0) {
			gameEntity->setCurrentStateType(RopeSpriteState::STATE_ROPE_SEPARATE);
		}
		else {
			gameEntity->setCurrentStateType(RopeSpriteState::STATE_ROPE_JUMP_ON);
		}
	}

	virtual void Receive(Message msg) {
		// if HIT by other game entities
		if (msg.getMessageType() == HIT) {
			// hit by mouse
			if (msg.getArg1()->getName() == CLASS_MOUSE) {
				SDL_Log("HIT ROPE!");
			}
		}
	}

	RopeSpriteState* getSpriteState() {
		return spriteState;
	}
	//void Destroy() {

	//}
private:

};