#pragma once
#include "sprite_state_interface.h"

class RoofSpriteState : public SpriteStateInterface {
public:
	const static int ROOF_TYPE_CORNER = 0;
	const static int ROOF_TYPE_CORNER_2 = 1;
	const static int ROOF_TYPE_PLAIN_LOW = 2;
	const static int ROOF_TYPE_WINDOW_SMALL = 3;
	const static int ROOF_TYPE_CURVE_UP = 4;
	const static int ROOF_TYPE_WINDOW_BIG = 5;
	const static int ROOF_TYPE_PLAIN_HIGH = 6;
	const static int ROOF_TYPE_DOOR = 7;

	const static int TOTAL_ROOF_TYPE = 8;

	const static int SPRITE_WIDTH = 40;
	const static int SPRITE_HEIGHT = 80;

	void Create(AvancezLib* system) {
		// Setup spritesheet
		SDL_Log("RoofSpriteSheet::Create");

		char* spriteName = "data/roof_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;

		// Init clip
		InitClip(0, 0, FRAME_NO_MOVE, corner);
		InitClip(1, 0, FRAME_NO_MOVE, corner2);
		InitClip(2, 0, FRAME_NO_MOVE, windowSmall);
		InitClip(3, 0, FRAME_NO_MOVE, plainLow);
		InitClip(4, 0, FRAME_NO_MOVE, curveUp);
		InitClip(5, 0, FRAME_NO_MOVE, windowBig);
		InitClip(6, 0, FRAME_NO_MOVE, plainHigh);
		InitClip(7, 0, FRAME_NO_MOVE, door);

		// Assign to states
		std::vector<EntityState*>* states = new std::vector<EntityState*>();

		states->push_back(CreateEntityState(ROOF_TYPE_CORNER, FRAME_NO_MOVE, corner));
		states->push_back(CreateEntityState(ROOF_TYPE_CORNER_2, FRAME_NO_MOVE, corner2));
		states->push_back(CreateEntityState(ROOF_TYPE_PLAIN_LOW, FRAME_NO_MOVE, windowSmall));
		states->push_back(CreateEntityState(ROOF_TYPE_WINDOW_SMALL, FRAME_NO_MOVE, plainLow));
		states->push_back(CreateEntityState(ROOF_TYPE_CURVE_UP, FRAME_NO_MOVE, curveUp));
		states->push_back(CreateEntityState(ROOF_TYPE_WINDOW_BIG, FRAME_NO_MOVE, windowBig));
		states->push_back(CreateEntityState(ROOF_TYPE_PLAIN_HIGH, FRAME_NO_MOVE, plainHigh));
		states->push_back(CreateEntityState(ROOF_TYPE_DOOR, FRAME_NO_MOVE, door));

		this->states = states;
	}

	int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}
private:
	// Set state and sprite clipping
	SDL_Rect corner[FRAME_NO_MOVE], corner2[FRAME_NO_MOVE], plainLow[FRAME_NO_MOVE], windowSmall[FRAME_NO_MOVE],
		curveUp[FRAME_NO_MOVE], plainHigh[FRAME_NO_MOVE], windowBig[FRAME_NO_MOVE], door[FRAME_NO_MOVE];
};