#pragma once
#include "game_entity.h"
#include "door_sprite_state.h"

class Door : public GameEntity {
public:

	bool isToggleEnabled = true;
	bool isOpen = false;
	int defaultState;

	void Create(float x, float y, int state) {
		GameEntity::Create(x, y);
		setCurrentStateType(state);
		defaultState = state;
	}
	/*virtual void Update(float dt) {

	}*/

	//virtual void setCurrentStateType(int currentStateType) {

	//}

	virtual void Init() {
		SDL_Log("Door::Init");
		GameEntity::Init();
		size->w = DoorSpriteState::SPRITE_WIDTH;
		size->h = DoorSpriteState::SPRITE_HEIGHT;
	}

	virtual std::string getName() {
		return CLASS_DOOR;
	}

	virtual GameEntity::Box getCollisionBox(SDL_Rect* camera = NULL) {
		GameEntity::Box collisionBox = GameEntity::getCollisionBox(camera);

		//override value
		/*collisionBox.y = verticalPosition + getSize()->h / 2;
		collisionBox.h = getSize()->h / 4;*/
		collisionBox.w = getSize()->w / 4;
		switch (getCurrentStateType()) {
			case  DoorSpriteState::STATE_DOOR_LEFT:
				collisionBox.x = horizontalPosition + getSize()->w - getSize()->w / 4;
				
				break;
			case  DoorSpriteState::STATE_DOOR_RIGHT:
				break;
			case  DoorSpriteState::STATE_DOOR_POWER_LEFT:
				collisionBox.x = horizontalPosition + getSize()->w - getSize()->w / 4;
				break;
			case  DoorSpriteState::STATE_DOOR_POWER_RIGHT:
				//collisionBox.w = horizontalPosition + +getSize()->w / 4;
				break;
		}

		

		return collisionBox;
	}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
	}
private:

};