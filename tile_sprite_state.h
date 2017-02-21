#pragma once
#include "sprite_state_interface.h"

class TileSpriteState : public SpriteStateInterface {
public:
	const static int STATE_TILE_WALL_RIGHT = 0;
	const static int STATE_TILE_CORNER_LEFT = 1;
	const static int STATE_TILE_PATH = 2;
	const static int STATE_TILE_CORNER_RIGHT = 3;
	const static int STATE_TILE_SPACE = 4;
	const static int STATE_TILE_BASE = 5;

	const static int TOTAL_TILE_SPRITES = 6;

	const static int TILE_WIDTH = 48;
	const static int TILE_HEIGHT = 48;

	void Create(AvancezLib* system);
private:
	const int spriteSize = 48;

	// Set state and sprite clipping
	SDL_Rect wallRightClips[FRAME_NO_MOVE];
	SDL_Rect cornerLeftClips[FRAME_NO_MOVE];
	SDL_Rect pathClips[FRAME_NO_MOVE];
	SDL_Rect cornerRightClips[FRAME_NO_MOVE];
	SDL_Rect spaceClips[FRAME_NO_MOVE];
	SDL_Rect baseClips[FRAME_NO_MOVE];
};