#pragma once
#include "game_entity.h"
#include "rope_sprite_state.h"
#include "rope_behavior_component.h"

class Rope : public GameEntity {
public:
	bool canJumpOn = true;
	//virtual void Create(int x, int y, int tileType) {	}
	/*virtual void Update(float dt) {
		
	}*/

	//virtual void setCurrentStateType(int currentStateType) {

	//}

	virtual void Init(float x, float y) {
		SDL_Log("Rope::Init");
		GameEntity::Init(x, y);

		setCurrentStateType(RopeSpriteState::STATE_ROPE_STATIC);

		size->w = RopeSpriteState::SPRITE_WIDTH;
		size->h = RopeSpriteState::SPRITE_HEIGHT;
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
			((RopeBehaviorComponent*)behaviorComponent)->restoreLife();
		}

	}
private:

};