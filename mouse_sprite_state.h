#pragma once
#include "sprite_state_interface.h"
class MouseSpriteState : public SpriteStateInterface {
public:
	const static int STATE_STAND = 0;
	const static int STATE_WALK = 1;
	const static int STATE_PREJUMP = 2;
	const static int STATE_INTHEAIR = 3;
	const static int STATE_KNOCKBACK = 4;
	const static int STATE_DEAD = 5;
	const static int STATE_STAND_RIGHT = 6;
	const static int STATE_WALK_RIGHT = 7;
	const static int STATE_PRE_JUMP_BACK = 8;
	const static int STATE_JUMP_BACK = 9;

	const static int STANDING_ANIMATION_FRAME = 1;
	const static int WALKING_ANIMATION_FRAME = 2;
	const static int WALKING_ANIMATION_INTERVAL = 0.5f;
	const static int PREJUMP_ANIMATION_FRAME = 1;
	const static int JUMP_ANIMATION_FRAME = 2;

	const static int SPRITE_WIDTH = 40;
	const static int SPRITE_HEIGHT = 40;

	void Create(AvancezLib* system);
private:
	const int spriteSize = 40;

	// Set state and sprite clipping
	SDL_Rect standSpriteClips[STANDING_ANIMATION_FRAME];
	SDL_Rect walkSpriteClips[WALKING_ANIMATION_FRAME];
	SDL_Rect prejumpSpriteClips[PREJUMP_ANIMATION_FRAME];
	SDL_Rect jumpSpriteClips[JUMP_ANIMATION_FRAME];
	SDL_Rect standRightSpriteClips[STANDING_ANIMATION_FRAME];
	SDL_Rect walkRightSpriteClips[WALKING_ANIMATION_FRAME];

	EntityState* jump;
	EntityState* walk;
	EntityState* stand;
};