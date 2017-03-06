#pragma once
#include "game_entity.h"
#include "door.h"
#include "door_behavior_component.h"

class DoorToggle : public GameEntity {
	std::vector<Door*>* doors;
	SDL_Rect* camera;

	float toggleInterval = 0.02f;
	float toggleTime = 0;
	bool canToggle = true;

public:
	void Create(std::vector<Door*>* doors, SDL_Rect* camera) {
		GameEntity::Create();
		this->doors = doors;
		this->camera = camera;
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
		GameEntity::Box controlEntityBox = controlEntity->getCollisionBox(camera);
		int xCheck = (int)controlEntityBox.x;
		Door* nearestDoor = nullptr;
		int minimumDistance = 9999;
		for (std::vector<Door*>::iterator it = doors->begin(); it != doors->end(); ++it) {
			Door* door = *it;
			if (door->isToggleEnabled) {
			
				GameEntity::Box doorBox = door->getCollisionBox();

				//check vertical
				if (!(controlEntityBox.y <= doorBox.y + doorBox.h && controlEntityBox.y + controlEntityBox.h >= doorBox.y)) {
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
			((DoorBehaviorComponent*)nearestDoor->getBehaviorComponent())->Toggle();
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