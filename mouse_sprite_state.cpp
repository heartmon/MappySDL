#pragma once
#include "mouse_sprite_state.h"

void MouseSpriteState::Create(AvancezLib* system) {
	// Set sprite sheet name
	SDL_Log("MouseSpriteState::Create");
	char* mappySpriteName = "data/mappy_sprite.png";
	SpriteSheet* mappySpriteSheet = system->createSpriteSheet(mappySpriteName);

	//setup clipping rect
	//stand animation
	standSpriteClips[0].x = spriteSize * 1;
	standSpriteClips[0].y = 0;
	standSpriteClips[0].w = spriteSize;
	standSpriteClips[0].h = spriteSize;
	stand = new EntityState(mappySpriteSheet, STATE_STAND, STANDING_ANIMATION_FRAME, &standSpriteClips[0]);

	//walking animation
	walkSpriteClips[0].x = spriteSize * 0;
	walkSpriteClips[0].y = 0;
	walkSpriteClips[0].w = spriteSize;
	walkSpriteClips[0].h = spriteSize;

	walkSpriteClips[1].x = spriteSize * 1;
	walkSpriteClips[1].y = 0;
	walkSpriteClips[1].w = spriteSize;
	walkSpriteClips[1].h = spriteSize;

	walk = new EntityState(mappySpriteSheet, STATE_WALK, WALKING_ANIMATION_FRAME, walkSpriteClips);

	//prejump animation

	//jump animation
	jumpSpriteClips[0].x = spriteSize * 3;
	jumpSpriteClips[0].y = 0;
	jumpSpriteClips[0].w = spriteSize;
	jumpSpriteClips[0].h = spriteSize;

	jumpSpriteClips[1].x = spriteSize * 4;
	jumpSpriteClips[1].y = 0;
	jumpSpriteClips[1].w = spriteSize;
	jumpSpriteClips[1].h = spriteSize;

	jump = new EntityState(mappySpriteSheet, STATE_JUMP, JUMP_ANIMATION_FRAME, jumpSpriteClips);

	std::vector<EntityState*>* states = new std::vector<EntityState*>;
	states->push_back(stand);
	states->push_back(walk);
	states->push_back(jump);
	
	// Set back
	this->states = states;
	SDL_Log("States after setback: %d", this->states->size());
}

std::vector<EntityState*>* MouseSpriteState::getSpriteStateEntities() {
	return states;
}

EntityState* MouseSpriteState::getMatchEntityState(int currentStateType) {
	switch (currentStateType) {
	case STATE_WALK:
		return walk;
	case STATE_STAND:
		return stand;
	default:
		return NULL;
	}
}