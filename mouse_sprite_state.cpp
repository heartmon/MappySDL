#pragma once
#include "mouse_sprite_state.h"

void MouseSpriteState::Create(AvancezLib* system) {
	// Set sprite sheet name
	char* mappySpriteName = "data/mappy_sprite.png";
	SpriteSheet* mappySpriteSheet = system->createSpriteSheet(mappySpriteName);

	//setup clipping rect
	//stand animation
	standSpriteClips[0].x = spriteSize * 1;
	standSpriteClips[0].y = 0;
	standSpriteClips[0].w = spriteSize;
	standSpriteClips[0].h = spriteSize;
	SDL_Rect* test = &standSpriteClips[0];
	SDL_Log("%d", test->x);
	EntityState* stand = new EntityState(mappySpriteSheet, STATE_STAND, STANDING_ANIMATION_FRAME, &standSpriteClips[0]);

	//walking animation
	walkSpriteClips[0].x = spriteSize * 0;
	walkSpriteClips[0].y = 0;
	walkSpriteClips[0].w = spriteSize;
	walkSpriteClips[0].h = spriteSize;

	walkSpriteClips[1].x = spriteSize * 1;
	walkSpriteClips[1].y = 0;
	walkSpriteClips[1].w = spriteSize;
	walkSpriteClips[1].h = spriteSize;

	EntityState* walk = new EntityState(mappySpriteSheet, STATE_WALK, WALKING_ANIMATION_FRAME, walkSpriteClips);

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

	EntityState* jump = new EntityState(mappySpriteSheet, STATE_JUMP, JUMP_ANIMATION_FRAME, jumpSpriteClips);

	std::set<EntityState*>* states = new std::set<EntityState*>;
	states->insert(stand);
	states->insert(walk);
	states->insert(jump);
	
	// Set back
	this->states = states;
	SDL_Log("States after setback: %d", this->states->size());
}

std::set<EntityState*>* MouseSpriteState::getSpriteStateEntities() {
	return states;
}