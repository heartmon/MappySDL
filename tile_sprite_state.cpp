#include "tile_sprite_state.h"

void TileSpriteState::Create(AvancezLib* system) {
	// Set sprite sheet name
	SDL_Log("TileSpritestate::Create");

	char* spriteName = "data/map_sprite.png";
	SpriteSheet* spriteSheet = system->createSpriteSheet(spriteName);

	wallRightClips[0].x = 0 * STATE_TILE_WALL_RIGHT;
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

	// Order is important
	EntityState* wallRight = new EntityState(spriteSheet, STATE_TILE_WALL_RIGHT, FRAME_NO_MOVE, wallRightClips);
	EntityState* cornerLeft = new EntityState(spriteSheet, STATE_TILE_CORNER_LEFT, FRAME_NO_MOVE, cornerLeftClips);
	EntityState* path = new EntityState(spriteSheet, STATE_TILE_PATH, FRAME_NO_MOVE, pathClips);
	EntityState* cornerRight = new EntityState(spriteSheet, STATE_TILE_CORNER_RIGHT, FRAME_NO_MOVE, cornerRightClips);
	EntityState* space = new EntityState(spriteSheet, STATE_TILE_SPACE, FRAME_NO_MOVE, spaceClips);
	EntityState* base = new EntityState(spriteSheet, STATE_TILE_BASE, FRAME_NO_MOVE, baseClips);

	std::vector<EntityState*>* states = new std::vector<EntityState*>;
	states->push_back(wallRight);
	states->push_back(cornerLeft);
	states->push_back(path);
	states->push_back(cornerRight);
	states->push_back(space);
	states->push_back(base);

	// Set back
	this->states = states;
}