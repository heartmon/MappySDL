#pragma once
#include "sprite_state_interface.h"

class ItemSpriteState : public SpriteStateInterface {
public:
	const static int STATE_ITEM_100 = 0;
	const static int STATE_ITEM_200 = 1;
	const static int STATE_ITEM_300 = 2;
	const static int STATE_ITEM_400 = 3;
	const static int STATE_ITEM_500 = 4;

	const static int STATE_ITEM_100_BONUS = 5;
	const static int STATE_ITEM_200_BONUS = 6;
	const static int STATE_ITEM_300_BONUS = 7;
	const static int STATE_ITEM_400_BONUS = 8;
	const static int STATE_ITEM_500_BONUS = 9;

	const static int FRAME_BONUS = 2;

	const static int SPRITE_WIDTH = 40;
	const static int SPRITE_HEIGHT = 40;

	virtual void Create(AvancezLib* system) {
		// Set sprite sheet name
		SDL_Log("ItemSpriteSheet::Create");

		char* spriteName = "data/item_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;

		

		InitClip(STATE_ITEM_100, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, ir100);
		InitClip(STATE_ITEM_200, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, ir200);
		InitClip(STATE_ITEM_300, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, ir300);
		InitClip(STATE_ITEM_400, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, ir400);
		InitClip(STATE_ITEM_500, SPRITE_HEIGHT * 0, FRAME_NO_MOVE, ir500);

		InitClip(STATE_ITEM_100, SPRITE_HEIGHT * 0, FRAME_BONUS, irb100);
		InitClip(STATE_ITEM_200, SPRITE_HEIGHT * 0, FRAME_BONUS, irb200);
		InitClip(STATE_ITEM_300, SPRITE_HEIGHT * 0, FRAME_BONUS, irb300);
		InitClip(STATE_ITEM_400, SPRITE_HEIGHT * 0, FRAME_BONUS, irb400);
		InitClip(STATE_ITEM_500, SPRITE_HEIGHT * 0, FRAME_BONUS, irb500);

		irb100[1].x = 5 * SPRITE_WIDTH;
		irb200[1].x = 5 * SPRITE_WIDTH;
		irb300[1].x = 5 * SPRITE_WIDTH;
		irb400[1].x = 5 * SPRITE_WIDTH;
		irb500[1].x = 5 * SPRITE_WIDTH;

		std::vector<EntityState*>* states = new std::vector<EntityState*>;
		states->push_back(CreateEntityState(STATE_ITEM_100, FRAME_NO_MOVE, ir100));
		states->push_back(CreateEntityState(STATE_ITEM_200, FRAME_NO_MOVE, ir200));
		states->push_back(CreateEntityState(STATE_ITEM_300, FRAME_NO_MOVE, ir300));
		states->push_back(CreateEntityState(STATE_ITEM_400, FRAME_NO_MOVE, ir400));
		states->push_back(CreateEntityState(STATE_ITEM_500, FRAME_NO_MOVE, ir500));

		states->push_back(CreateEntityState(STATE_ITEM_100_BONUS, FRAME_BONUS, irb100));
		states->push_back(CreateEntityState(STATE_ITEM_200_BONUS, FRAME_BONUS, irb200));
		states->push_back(CreateEntityState(STATE_ITEM_300_BONUS, FRAME_BONUS, irb300));
		states->push_back(CreateEntityState(STATE_ITEM_400_BONUS, FRAME_BONUS, irb400));
		states->push_back(CreateEntityState(STATE_ITEM_500_BONUS, FRAME_BONUS, irb500));

		this->states = states;

	}

	virtual int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	virtual int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}


private:
	SDL_Rect ir100[FRAME_NO_MOVE], ir200[FRAME_NO_MOVE], ir300[FRAME_NO_MOVE], ir400[FRAME_NO_MOVE], ir500[FRAME_NO_MOVE];
	SDL_Rect irb100[FRAME_BONUS], irb200[FRAME_BONUS], irb300[FRAME_BONUS], irb400[FRAME_BONUS], irb500[FRAME_BONUS];
};