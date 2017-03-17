#pragma once
#include "game_entity.h"
#include "roof_sprite_state.h"

class Roof : public GameEntity {

public:
	void Create() {
		GameEntity::Create();
		size->w = RoofSpriteState::SPRITE_WIDTH;
		size->h = RoofSpriteState::SPRITE_HEIGHT;
	}

	void Init(float x, float y, int tileType, int direction) {
		GameEntity::Init(x, y);


		currentStateType = tileType;
		this->direction = direction;

	}

};