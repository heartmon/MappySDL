#pragma once
#include "sprite_state_interface.h"

class DoorSpriteState : public SpriteStateInterface {
public:
	const static int STATE_DOOR_LEFT = 0;
	const static int STATE_DOOR_RIGHT = 1;
	const static int STATE_DOOR_POWER_LEFT = 2;
	const static int STATE_DOOR_POWER_RIGHT = 3;
	const static int STATE_DOOR_OPEN = 4;



	const static int FRAME_DOOR_POWER = 2;

	const static int SPRITE_WIDTH = 40;
	const static int SPRITE_HEIGHT = 65;

	virtual void Create(AvancezLib* system) {
		// Set sprite sheet name
		SDL_Log("DoorSpriteState::Create");

		char* spriteName = "data/door_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;



		InitClip(0, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, dl);
		InitClip(2, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, dr);

		InitClip(4, SPRITE_HEIGHT * 0, FRAME_DOOR_POWER, dpl);
		InitClip(6, SPRITE_HEIGHT * 0, FRAME_DOOR_POWER, dpr);

		InitClip(1, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, dopen);

		std::vector<EntityState*>* states = new std::vector<EntityState*>;
		states->push_back(CreateEntityState(STATE_DOOR_LEFT, FRAME_NO_MOVE, dl));
		states->push_back(CreateEntityState(STATE_DOOR_RIGHT, FRAME_NO_MOVE, dr));
		states->push_back(CreateEntityState(STATE_DOOR_POWER_LEFT, FRAME_DOOR_POWER, dpl));
		states->push_back(CreateEntityState(STATE_DOOR_POWER_RIGHT, FRAME_DOOR_POWER, dpr));
		states->push_back(CreateEntityState(STATE_DOOR_OPEN, FRAME_NO_MOVE, dopen));

		this->states = states;

	}

	virtual int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	virtual int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}

	bool static isPowerDoor(int state) {
		switch (state) {
			case STATE_DOOR_POWER_LEFT:
			case STATE_DOOR_POWER_RIGHT:
				return true;
		}

		return false;
	}


private:
	SDL_Rect dl[FRAME_NO_MOVE], dr[FRAME_NO_MOVE], dpl[FRAME_DOOR_POWER], dpr[FRAME_DOOR_POWER], dopen[FRAME_NO_MOVE];
};