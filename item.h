#pragma once
#include "game_entity.h"
#include "item_sprite_state.h"
#include "item_behavior_component.h"

class Item : public GameEntity {
public:

	void Create(float x, float y, int state, int baseScore) {
		GameEntity::Create(x, y);
		setCurrentStateType(state);
		this->baseScore = baseScore;
	}
	/*virtual void Update(float dt) {

	}*/

	//virtual void setCurrentStateType(int currentStateType) {

	//}

	virtual void Init() {
		SDL_Log("Item::Init");
		GameEntity::Init();
		size->w = ItemSpriteState::SPRITE_WIDTH;
		size->h = ItemSpriteState::SPRITE_HEIGHT;
	}

	virtual std::string getName() {
		return CLASS_ITEM;
	}

	//virtual GameEntity::Box getCollisionBox(SDL_Rect* camera = NULL) {
	//	GameEntity::Box colBox = GameEntity::getCollisionBox(camera);

	//	//override value
	//	colBox.y = verticalPosition + getSize()->h / 2;

	//	return colBox;
	//}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
	}

	int getBaseScore() {
		return baseScore;
	}

private:
	int baseScore;
};