#pragma once
#include "component.h"
#include "message.h"
#include "global_constant.h"
#include "door.h"

class DoorBehaviorComponent : public Component {
	std::vector<Door*>* allDoors;
	SDL_Rect* camera;
	float toggleInterval = .25f;
	float toggleTime = 0;
	Door* gameEntity;

public:
	virtual void Create(AvancezLib* system, Door* go, std::vector<Door*> * allDoors, SDL_Rect* camera) {
		this->system = system;
		this->gameEntity = go;
		this->camera = camera;
		this->allDoors = allDoors;
	}
	virtual void Init() {

	}


	void Update(float dt) {
		toggleTime += dt;

		if (toggleTime > toggleInterval) {
			gameEntity->isToggleEnabled = true;
			toggleTime = 0;
		}
	}

	void Toggle() {
		SDL_Log("Toggle Work ..");
		gameEntity->isOpen = !gameEntity->isOpen;

		UpdateState();
	}

	void UpdateState() {
		if (gameEntity->isOpen) {
			if (gameEntity->defaultState == DoorSpriteState::STATE_DOOR_POWER_LEFT) {
				SDL_Log("Send release rainbow message");
				gameEntity->Send(new Message(RELEASE_RAINBOW, gameEntity));
				gameEntity->defaultState = DoorSpriteState::STATE_DOOR_LEFT;
				
			}
			if (gameEntity->defaultState == DoorSpriteState::STATE_DOOR_POWER_RIGHT) {
				SDL_Log("Send release rainbow message");
				gameEntity->Send(new Message(RELEASE_RAINBOW, gameEntity));
				gameEntity->defaultState = DoorSpriteState::STATE_DOOR_RIGHT;
				
			}
			gameEntity->setCurrentStateType(DoorSpriteState::STATE_DOOR_OPEN);
			gameEntity->Send(new Message(DOOR_OPEN, gameEntity));
		}
		else {
			gameEntity->setCurrentStateType(gameEntity->defaultState);
			gameEntity->Send(new Message(DOOR_CLOSE, gameEntity));
		}
	}

	virtual void Receive(Message msg) {
		// if HIT by other game entities
		if (msg.getMessageType() == HIT) {
			// hit by mouse
			if (msg.getArg1()->getName() == CLASS_MOUSE) {
				SDL_Log("HIT DOOR!");
			}
		}
	}

	//void Destroy() {

	//}
private:

};