#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "big_cat_sprite_state.h"

class BigCat : public GameEntity {
public:
	int lives;

	//
	virtual void Create() {
		GameEntity::Create();
		size->w = BigCatSpriteState::SPRITE_WIDTH;
		size->h = BigCatSpriteState::SPRITE_HEIGHT;
	}
	virtual void Init()
	{
		SDL_Log("BigCat::Init");
		GameEntity::Init();
		vy = 0;
		vx = 160.0f;
		setCurrentStateType(BigCatSpriteState::STATE_INTHEAIR);
	}
	virtual void Init(float x, float y) {
		Init();
		horizontalPosition = x;
		verticalPosition = y;
	}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
		behaviorComponent->Receive(m);
	}

	virtual std::string getName() {
		return CLASS_CAT;
	}

	virtual int getWidth() {
		return BigCatSpriteState::SPRITE_WIDTH;
	}

	virtual int getHeight() {
		return BigCatSpriteState::SPRITE_HEIGHT;
	}

private:

};