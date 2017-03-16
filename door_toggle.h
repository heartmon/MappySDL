#pragma once
#include "game_entity.h"
#include "door.h"
#include "door_behavior_component.h"

class DoorToggle : public GameEntity {
	ObjectPool<Door>* doors;
	SDL_Rect* camera;

	float toggleInterval = 0.2f;
	float toggleTime = 0;
	bool canToggle = true;

public:
	void Create(ObjectPool<Door>* doors, SDL_Rect* camera) {
		GameEntity::Create();
		this->doors = doors;
		this->camera = camera;
	}

	void Init() {
		GameEntity::Init();
		SDL_Log("DoorController:: Init");
	}

	void Update(float dt) {
		toggleTime += dt;
		if (toggleTime > toggleInterval) {
			toggleTime = 0;
			canToggle = true;
		}
	}

	void ToggleCheck(GameEntity* controlEntity) {
		if (!canToggle) {
			return;
		}
		canToggle = false;
		GameEntity::Box controlEntityBox;

		bool isLimitTheRange = false;

		if (controlEntity->getName() == CLASS_MOUSE) {
			controlEntityBox = controlEntity->getCollisionBox(camera);
		}
		else {
			controlEntityBox = controlEntity->getCollisionBox();
		}
		int xCheck = (int)controlEntityBox.x;
		Door* nearestDoor = nullptr;
		int minimumDistance = 9999;
		for (auto it = doors->pool.begin(); it != doors->pool.end(); ++it) {
			if (!(*it)->enabled) continue;

			Door* door = *it;
			if (door->isToggleEnabled) {
			
				GameEntity::Box doorBox = door->getCollisionBox();

				//check vertical
				if (!(controlEntityBox.y <= doorBox.y + doorBox.h && controlEntityBox.y + controlEntityBox.h >= doorBox.y)) {
					continue;
				}

				//check limited range
				if (abs(controlEntityBox.x - doorBox.x) > SCREEN_WIDTH / 2 && controlEntity->getName() != CLASS_MOUSE) {
					continue;
				}
				
				if (controlEntity->direction == GameEntity::LEFT) {
					if (xCheck - doorBox.x < minimumDistance && xCheck - doorBox.x > 0 && doorBox.x > camera->x) {
						nearestDoor = door;
						minimumDistance = xCheck - doorBox.x;
					}
				}
				else if (controlEntity->direction == GameEntity::RIGHT) {
					if (doorBox.x - xCheck < minimumDistance && doorBox.x - xCheck > 0 && doorBox.x < camera->x + camera->w) {
						nearestDoor = door;
						minimumDistance = doorBox.x - xCheck;
					}
				}
			}
			else {
				//SDL_Log("Door delay");
			}
		}

		if (nearestDoor != nullptr) {
			nearestDoor->isToggleEnabled = false;
			/*SDL_Log("WORKKKKKKKKKKKKKK LEFT");*/
			if ((nearestDoor->getCurrentStateType() == DoorSpriteState::STATE_DOOR_POWER_LEFT
				|| nearestDoor->getCurrentStateType() == DoorSpriteState::STATE_DOOR_POWER_RIGHT)
				&& controlEntity->getName() != CLASS_MOUSE
				) {
				// cannot toggle this door
				return;
			}
			((DoorBehaviorComponent*)nearestDoor->getBehaviorComponent())->Toggle();

			if (controlEntity->getName() == CLASS_MOUSE) {
				SDL_Log("MOUSE TOGGLE DOOR");
			}
		}
		
	}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == TOGGLE_DOOR) {
			ToggleCheck(m->getArg1());
			//((DoorBehaviorComponent*)behaviorComponent)->Toggle(m->getArg1());
		}
	}
private:

};