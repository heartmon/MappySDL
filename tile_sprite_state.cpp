#include "tile_sprite_state.h"

void TileSpriteState::Create(AvancezLib* system) {
	// Set sprite sheet name
	SDL_Log("TileSpritestate::Create");

	char* spriteName = "data/map_sprite.png";
	SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);
	this->spriteSheet = spriteSheet;

	wallRightClips[0].x = TILE_WIDTH * STATE_TILE_WALL_RIGHT;
	wallRightClips[0].y = 0;
	wallRightClips[0].w = TILE_WIDTH;
	wallRightClips[0].h = TILE_HEIGHT;

	cornerLeftClips[0].x = TILE_WIDTH * STATE_TILE_CORNER_LEFT;
	cornerLeftClips[0].y = 0;
	cornerLeftClips[0].w = TILE_WIDTH;
	cornerLeftClips[0].h = TILE_HEIGHT;

	pathClips[0].x = TILE_WIDTH * STATE_TILE_PATH;
	pathClips[0].y = 0;
	pathClips[0].w = TILE_WIDTH;
	pathClips[0].h = TILE_HEIGHT;

	cornetCenterClips[0].x = TILE_WIDTH * STATE_TILE_CORNER_CENTER;
	cornetCenterClips[0].y = 0;
	cornetCenterClips[0].w = TILE_WIDTH;
	cornetCenterClips[0].h = TILE_HEIGHT;

	cornerRightClips[0].x = TILE_WIDTH * STATE_TILE_CORNER_RIGHT;
	cornerRightClips[0].y = 0;
	cornerRightClips[0].w = TILE_WIDTH;
	cornerRightClips[0].h = TILE_HEIGHT;

	spaceClips[0].x = TILE_WIDTH * STATE_TILE_SPACE;
	spaceClips[0].y = 0;
	spaceClips[0].w = TILE_WIDTH;
	spaceClips[0].h = TILE_HEIGHT;

	baseClips[0].x = TILE_WIDTH * STATE_TILE_BASE;
	baseClips[0].y = 0;
	baseClips[0].w = TILE_WIDTH;
	baseClips[0].h = TILE_HEIGHT;

	wallLeftClips[0].x = TILE_WIDTH * STATE_TILE_WALL_LEFT;
	wallLeftClips[0].y = 0;
	wallLeftClips[0].w = TILE_WIDTH;
	wallLeftClips[0].h = TILE_HEIGHT;

	// Order is important
	EntityState* space = new EntityState(spriteSheet, STATE_TILE_SPACE, FRAME_NO_MOVE, spaceClips);
	EntityState* cornerLeft = new EntityState(spriteSheet, STATE_TILE_CORNER_LEFT, FRAME_NO_MOVE, cornerLeftClips);
	EntityState* path = new EntityState(spriteSheet, STATE_TILE_PATH, FRAME_NO_MOVE, pathClips);
	EntityState* cornerCenter = new EntityState(spriteSheet, STATE_TILE_CORNER_CENTER, FRAME_NO_MOVE, cornetCenterClips);
	EntityState* cornerRight = new EntityState(spriteSheet, STATE_TILE_CORNER_RIGHT, FRAME_NO_MOVE, cornerRightClips);
	EntityState* base = new EntityState(spriteSheet, STATE_TILE_BASE, FRAME_NO_MOVE, baseClips);
	EntityState* wallLeft = new EntityState(spriteSheet, STATE_TILE_WALL_LEFT, FRAME_NO_MOVE, wallLeftClips);
	EntityState* wallRight = new EntityState(spriteSheet, STATE_TILE_WALL_RIGHT, FRAME_NO_MOVE, wallRightClips);
		
	EntityState* item100 = new EntityState(spriteSheet, STATE_TILE_ITEM_100, FRAME_NO_MOVE, spaceClips);
	EntityState* item200 = new EntityState(spriteSheet, STATE_TILE_ITEM_200, FRAME_NO_MOVE, spaceClips);
	EntityState* item300 = new EntityState(spriteSheet, STATE_TILE_ITEM_300, FRAME_NO_MOVE, spaceClips);
	EntityState* item400 = new EntityState(spriteSheet, STATE_TILE_ITEM_400, FRAME_NO_MOVE, spaceClips);
	EntityState* item500 = new EntityState(spriteSheet, STATE_TILE_ITEM_500, FRAME_NO_MOVE, spaceClips);

	EntityState* rope = new EntityState(spriteSheet, STATE_TILE_ROPE, FRAME_NO_MOVE, spaceClips);
	EntityState* doorL = new EntityState(spriteSheet, STATE_TILE_DOOR_LEFT, FRAME_NO_MOVE, spaceClips);
	EntityState* doorR = new EntityState(spriteSheet, STATE_TILE_DOOR_RIGHT, FRAME_NO_MOVE, spaceClips);
	EntityState* doorPL = new EntityState(spriteSheet, STATE_TILE_DOOR_POWER_LEFT, FRAME_NO_MOVE, spaceClips);
	EntityState* doorPR = new EntityState(spriteSheet, STATE_TILE_DOOR_POWER_RIGHT, FRAME_NO_MOVE, spaceClips);

	std::vector<EntityState*>* states = new std::vector<EntityState*>;
	states->push_back(space);
	states->push_back(cornerLeft);
	states->push_back(path);
	states->push_back(cornerCenter);
	states->push_back(cornerRight);
	states->push_back(base);
	
	states->push_back(wallRight);
	states->push_back(wallLeft);

	states->push_back(space);
	states->push_back(space);
	states->push_back(space);

	states->push_back(item100);
	states->push_back(item200);
	states->push_back(item300);
	states->push_back(item400);
	states->push_back(item500);

	states->push_back(space);
	states->push_back(space);
	states->push_back(space);
	states->push_back(space);
	states->push_back(space);
	
	states->push_back(rope);
	states->push_back(doorL);
	states->push_back(doorR);
	states->push_back(doorPL);
	states->push_back(doorPR);
	
	

	// Set back
	this->states = states;
}