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
	const static int STATE_TILE_WALL_LEFT = 6;
	const static int STATE_TILE_SPACE_JUMP = 7;
	const static int STATE_TILE_ROPE = 8;

	const static int TOTAL_TILE_SPRITES = 9;

	const static int TILE_WIDTH = 40;
	const static int TILE_HEIGHT = 40;

	void Create(AvancezLib* system);

	bool static isFloor(int state) {
		switch (state) {
			case STATE_TILE_CORNER_RIGHT:
			case STATE_TILE_CORNER_LEFT:
			case STATE_TILE_PATH:
			case STATE_TILE_BASE:
				return true;
		}
		return false;
	}
private:
	const int spriteSize = 40;

	// Set state and sprite clipping
	SDL_Rect wallRightClips[FRAME_NO_MOVE];
	SDL_Rect cornerLeftClips[FRAME_NO_MOVE];
	SDL_Rect pathClips[FRAME_NO_MOVE];
	SDL_Rect cornerRightClips[FRAME_NO_MOVE];
	SDL_Rect spaceClips[FRAME_NO_MOVE];
	SDL_Rect baseClips[FRAME_NO_MOVE];
	SDL_Rect wallLeftClips[FRAME_NO_MOVE];
	SDL_Rect spaceJumpClips[FRAME_NO_MOVE];
};