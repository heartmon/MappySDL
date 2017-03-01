#pragma once
#include "game_entity.h"
#include "item_sprite_state.h"
#include "item_behavior_component.h"

class Item : public GameEntity {
public:

	//virtual void Create(int x, int y, int tileType) {	}
	/*virtual void Update(float dt) {

	}*/

	//virtual void setCurrentStateType(int currentStateType) {

	//}

	virtual void Init() {
		SDL_Log("Rope::Init");
		GameEntity::Init();

		setCurrentStateType(ItemSpriteState::STATE_ROPE_STATIC);

		size->w = ItemSpriteState::SPRITE_WIDTH;
		size->h = ItemSpriteState::SPRITE_HEIGHT;
	}

	virtual std::string getName() {
		return CLASS_ROPE;
	}

	virtual GameEntity::Box getCollisionBox(SDL_Rect* camera = NULL) {
		GameEntity::Box ropeCollisionBox = GameEntity::getCollisionBox(camera);

		//override value
		ropeCollisionBox.y = verticalPosition + getSize()->h / 2;

		return ropeCollisionBox;
	}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == MOUSE_JUMP_TO_STAND) {
			//((RopeBehaviorComponent*)behaviorComponent)->restoreLife();
		}

	}
private:

};