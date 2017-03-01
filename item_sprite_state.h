#pragma once
#include "sprite_state_interface.h"

class ItemSpriteState : public SpriteStateInterface {
public:
	const static int STATE_ROPE_STATIC = 0;
	const static int STATE_ROPE_JUMP_ON = 1;
	const static int STATE_ROPE_SEPARATE = 2;

	const static int FRAME_ROPE_JUMP_ON = 12;

	const static int SPRITE_WIDTH = 80;
	const static int SPRITE_HEIGHT = 40;

	virtual void Create(AvancezLib* system) {
		// Set sprite sheet name
		SDL_Log("RopeSpriteSheet::Create");

		char* spriteName = "data/rope_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;


		InitClip(STATE_ROPE_STATIC, SPRITE_HEIGHT * 2, FRAME_NO_MOVE, staticClip);
		InitClip(STATE_ROPE_JUMP_ON, SPRITE_HEIGHT * 2, FRAME_ROPE_JUMP_ON, jumpOnClip);
		jumpOnClip[0].x = SPRITE_WIDTH * 1;
		jumpOnClip[1].x = SPRITE_WIDTH * 2;
		jumpOnClip[2].x = SPRITE_WIDTH * 3;
		jumpOnClip[3].x = SPRITE_WIDTH * 2;
		jumpOnClip[4].x = SPRITE_WIDTH * 1;
		jumpOnClip[5].x = SPRITE_WIDTH * 0;
		jumpOnClip[6].x = SPRITE_WIDTH * 4;
		jumpOnClip[7].x = SPRITE_WIDTH * 5;
		jumpOnClip[8].x = SPRITE_WIDTH * 6;
		jumpOnClip[9].x = SPRITE_WIDTH * 5;
		jumpOnClip[10].x = SPRITE_WIDTH * 4;
		jumpOnClip[11].x = SPRITE_WIDTH * 0;

		InitClip(7, SPRITE_HEIGHT * 3, FRAME_NO_MOVE, separateClip);

		std::vector<EntityState*>* states = new std::vector<EntityState*>;
		states->push_back(CreateEntityState(STATE_ROPE_STATIC, FRAME_NO_MOVE, staticClip));
		states->push_back(CreateEntityState(STATE_ROPE_JUMP_ON, FRAME_ROPE_JUMP_ON, jumpOnClip, 70));
		states->push_back(CreateEntityState(STATE_ROPE_SEPARATE, FRAME_NO_MOVE, separateClip));

		this->states = states;

	}

	virtual int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	virtual int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}


private:
	SDL_Rect staticClip[FRAME_NO_MOVE];
	SDL_Rect jumpOnClip[FRAME_ROPE_JUMP_ON];
	SDL_Rect separateClip[FRAME_NO_MOVE];
};