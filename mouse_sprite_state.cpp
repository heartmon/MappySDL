#pragma once
#include "mouse_sprite_state.h"

void MouseSpriteState::Create(AvancezLib* system) {
	// Set sprite sheet name
	SDL_Log("MouseSpriteState::Create");
	char* mappySpriteName = "data/mappy_sprite3.png";
	SpriteSheet* mappySpriteSheet = system->createSpriteSheet(mappySpriteName);

	//setup clipping rect
	//stand animation
	standSpriteClips[0].x = spriteSize * 1;
	standSpriteClips[0].y = 0;
	standSpriteClips[0].w = spriteSize;
	standSpriteClips[0].h = spriteSize;
	stand = new EntityState(mappySpriteSheet, STATE_STAND, STANDING_ANIMATION_FRAME, standSpriteClips);

	//stand animation
	/*standRightSpriteClips[0].x = spriteSize * 1;
	standRightSpriteClips[0].y = spriteSize * 1;
	standRightSpriteClips[0].w = spriteSize;
	standRightSpriteClips[0].h = spriteSize;
	EntityState* standRight = new EntityState(mappySpriteSheet, STATE_STAND_RIGHT, STANDING_ANIMATION_FRAME, standRightSpriteClips);*/

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

	/*walkRightSpriteClips[0].x = spriteSize * 0;
	walkRightSpriteClips[0].y = spriteSize * 1;
	walkRightSpriteClips[0].w = spriteSize;
	walkRightSpriteClips[0].h = spriteSize;*/

	/*walkRightSpriteClips[1].x = spriteSize * 1;
	walkRightSpriteClips[1].y = spriteSize * 1;
	walkRightSpriteClips[1].w = spriteSize;
	walkRightSpriteClips[1].h = spriteSize;*/

	//EntityState* walkRight = new EntityState(mappySpriteSheet, STATE_WALK_RIGHT, WALKING_ANIMATION_FRAME, walkRightSpriteClips);

	//prejump animation
	prejumpSpriteClips[0].x = spriteSize * 6;
	prejumpSpriteClips[0].y = 0;
	prejumpSpriteClips[0].w = spriteSize;
	prejumpSpriteClips[0].h = spriteSize;
	EntityState* preJump = new EntityState(mappySpriteSheet, STATE_PREJUMP, PREJUMP_ANIMATION_FRAME, prejumpSpriteClips);

	//jump animation
	jumpSpriteClips[0].x = spriteSize * 3;
	jumpSpriteClips[0].y = 0;
	jumpSpriteClips[0].w = spriteSize;
	jumpSpriteClips[0].h = spriteSize;

	jumpSpriteClips[1].x = spriteSize * 4;
	jumpSpriteClips[1].y = 0;
	jumpSpriteClips[1].w = spriteSize;
	jumpSpriteClips[1].h = spriteSize;

	jump = new EntityState(mappySpriteSheet, STATE_INTHEAIR, JUMP_ANIMATION_FRAME, jumpSpriteClips);
	//new code
	InitClip(5, 0, FRAME_NO_MOVE, knockbackClip);

	//dead
	InitClip(7, 0, DEAD_ANIMATION_FRAME, deadClip);
	InitClip(16, 0, DEAD_MOVING_ANIMATION_FRAME, deadMovingClip);

	EntityState* preJumpBack = new EntityState(mappySpriteSheet, STATE_PRE_JUMP_BACK, JUMP_ANIMATION_FRAME, jumpSpriteClips);
	EntityState* jumpBack = new EntityState(mappySpriteSheet, STATE_JUMP_BACK, PREJUMP_ANIMATION_FRAME, prejumpSpriteClips);
	EntityState* knockBack = new EntityState(mappySpriteSheet, STATE_KNOCKBACK, FRAME_NO_MOVE, knockbackClip);
	EntityState* dead = new EntityState(mappySpriteSheet, STATE_DEAD, DEAD_ANIMATION_FRAME, deadClip, 14.0f);
	EntityState* deadMoving= new EntityState(mappySpriteSheet, STATE_DEAD_MOVING, DEAD_MOVING_ANIMATION_FRAME, deadMovingClip, 25.0f);

	std::vector<EntityState*>* states = new std::vector<EntityState*>;
	states->push_back(stand);
	states->push_back(walk);
	states->push_back(jump);
	states->push_back(knockBack);
	/*states->push_back(standRight);
	states->push_back(walkRight);*/
	states->push_back(preJump);
	states->push_back(preJumpBack);
	states->push_back(jumpBack);
	states->push_back(dead);
	states->push_back(deadMoving);

	
	
	// Set back
	this->states = states;
	SDL_Log("States after setback: %d", this->states->size());
}