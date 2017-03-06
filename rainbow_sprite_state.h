#pragma once
#include "sprite_state_interface.h"

class RainbowSpriteState : public SpriteStateInterface {
public:
	const static int STATE_RAINBOW = 0;

	const static int FRAME_RAINBOW = 4;

	const static int SPRITE_WIDTH = 46;
	const static int SPRITE_HEIGHT = 82;

	virtual void Create(AvancezLib* system) {
		// Set sprite sheet name
		SDL_Log("RainbowSpriteState::Create");

		char* spriteName = "data/rainbow_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;


		InitClip(STATE_RAINBOW, SPRITE_HEIGHT * 0, FRAME_RAINBOW, rainbowClip);

		std::vector<EntityState*>* states = new std::vector<EntityState*>;
		states->push_back(CreateEntityState(STATE_RAINBOW, FRAME_RAINBOW, rainbowClip));

		this->states = states;

	}

	virtual int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	virtual int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}


private:
	SDL_Rect rainbowClip[FRAME_RAINBOW];
};