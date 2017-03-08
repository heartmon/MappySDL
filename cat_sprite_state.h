#pragma once
#include "sprite_state_interface.h"
class CatSpriteState : public SpriteStateInterface {
public:
	const static int STATE_STAND = 0;
	const static int STATE_WALK = 1;
	const static int STATE_PREJUMP = 2;
	const static int STATE_INTHEAIR = 3;
	const static int STATE_KNOCKBACK = 4;
	const static int STATE_AFTER_KNOCKBACK = 5;
	const static int STATE_PRE_JUMP_BACK = 6;
	const static int STATE_JUMP_BACK = 7;

	const static int STANDING_ANIMATION_FRAME = 1;
	const static int WALKING_ANIMATION_FRAME = 2;
	const static int PREJUMP_ANIMATION_FRAME = 1;
	const static int INTHEAIR_ANIMATION_FRAME = 2;
	const static int KNOCKBACK_FRAME = 1;
	const static int AFTER_KNOCKBACK_FRAME = 2;

	const static int SPRITE_WIDTH = 40;
	const static int SPRITE_HEIGHT = 40;

	void Create(AvancezLib* system) {
		// Setup spritesheet
		SDL_Log("CatSpriteSheet::Create");

		char* spriteName = "data/cat_sprite.png";
		SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
		this->spriteSheet = spriteSheet;

		// Init clip
		InitClip(0, 0, STANDING_ANIMATION_FRAME, standClip);
		InitClip(1, 0, WALKING_ANIMATION_FRAME, walkClip);
		InitClip(2, 0, PREJUMP_ANIMATION_FRAME, preJumpClip);
		InitClip(3, 0, INTHEAIR_ANIMATION_FRAME, intheairClip);
		InitClip(5, 0, KNOCKBACK_FRAME, knockbackClip);
		InitClip(5, 0, AFTER_KNOCKBACK_FRAME, afterKnockbackClip);

		// Assign to states
		std::vector<EntityState*>* states = new std::vector<EntityState*>();

		states->push_back(CreateEntityState(STATE_STAND, STANDING_ANIMATION_FRAME, standClip));
		states->push_back(CreateEntityState(STATE_WALK, WALKING_ANIMATION_FRAME, walkClip));
		states->push_back(CreateEntityState(STATE_PREJUMP, PREJUMP_ANIMATION_FRAME, preJumpClip));
		states->push_back(CreateEntityState(STATE_INTHEAIR, INTHEAIR_ANIMATION_FRAME, intheairClip));
		states->push_back(CreateEntityState(STATE_KNOCKBACK, KNOCKBACK_FRAME, knockbackClip));
		states->push_back(CreateEntityState(STATE_AFTER_KNOCKBACK, AFTER_KNOCKBACK_FRAME, afterKnockbackClip));

		states->push_back(CreateEntityState(STATE_PRE_JUMP_BACK, INTHEAIR_ANIMATION_FRAME, intheairClip));
		states->push_back(CreateEntityState(STATE_JUMP_BACK, PREJUMP_ANIMATION_FRAME, preJumpClip));

		this->states = states;
	}

	int getSpriteWidth() {
		return SPRITE_WIDTH;
	}
	int getSpriteHeight() {
		return SPRITE_HEIGHT;
	}
private:
	const int spriteSize = 40;

	// Set state and sprite clipping
	SDL_Rect standClip[STANDING_ANIMATION_FRAME];
	SDL_Rect walkClip[WALKING_ANIMATION_FRAME];
	SDL_Rect preJumpClip[PREJUMP_ANIMATION_FRAME];
	SDL_Rect intheairClip[INTHEAIR_ANIMATION_FRAME];
	SDL_Rect knockbackClip[KNOCKBACK_FRAME];
	SDL_Rect afterKnockbackClip[AFTER_KNOCKBACK_FRAME];

};