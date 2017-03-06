#pragma once
#include "game_entity.h"
#include "rainbow_sprite_state.h"

class Rainbow : public GameEntity {

public:
	virtual void Create(float x, float y, int direction) {
		GameEntity::Create();
		horizontalPosition = x;
		verticalPosition = y;
		this->direction = direction;
	}


	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
	}

	virtual void Init() {
		SDL_Log("Rainbow::Init");
		GameEntity::Init();
		size->w = RainbowSpriteState::SPRITE_WIDTH;
		size->h = RainbowSpriteState::SPRITE_HEIGHT;

		if (direction == GameEntity::LEFT) {
			setCurrentStateType(RainbowSpriteState::STATE_RAINBOW);
		}
		else {
			setCurrentStateType(RainbowSpriteState::STATE_RAINBOW);
		}
	}

	virtual std::string getName() {
		return CLASS_RAINBOW;
	}
private:

};