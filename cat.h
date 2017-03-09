#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "cat_sprite_state.h"

class Cat : public GameEntity {
public:
	int lives;

	//
	virtual void Create() {
		GameEntity::Create();
		size->w = CatSpriteState::SPRITE_WIDTH;
		size->h = CatSpriteState::SPRITE_HEIGHT;
	}
	virtual void Init()
	{
		SDL_Log("Cat::Init");
		GameEntity::Init();
		vy = 0;
		vx = 160.0f;
		setCurrentStateType(CatSpriteState::STATE_INTHEAIR);
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
		return CatSpriteState::SPRITE_WIDTH;
	}

	virtual int getHeight() {
		return CatSpriteState::SPRITE_HEIGHT;
	}

private:

};