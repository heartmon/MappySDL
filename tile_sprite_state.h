#pragma once
#include "sprite_state_interface.h"

class TileSpriteState : public SpriteStateInterface {
public:
	const static int STATE_TILE_SPACE = 0;
	const static int STATE_TILE_CORNER_LEFT = 1;
	const static int STATE_TILE_PATH = 2;
	const static int STATE_TILE_CORNER_CENTER = 3;
	const static int STATE_TILE_CORNER_RIGHT = 4;
	const static int STATE_TILE_BASE = 5;
	const static int STATE_TILE_WALL_RIGHT = 6;
	const static int STATE_TILE_WALL_LEFT = 7;

	const static int STATE_TILE_ITEM_100 = 11;
	const static int STATE_TILE_ITEM_200 = 12;
	const static int STATE_TILE_ITEM_300 = 13;
	const static int STATE_TILE_ITEM_400 = 14;
	const static int STATE_TILE_ITEM_500 = 15;
	
	const static int STATE_TILE_ROPE = 21;
	const static int STATE_TILE_DOOR = 22;

	
	//const static int TOTAL_TILE_SPRITES = 9;

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

	virtual int getSpriteWidth() {
		return TILE_WIDTH;
	}
	virtual int getSpriteHeight() {
		return TILE_HEIGHT;
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
	SDL_Rect cornetCenterClips[FRAME_NO_MOVE];
};