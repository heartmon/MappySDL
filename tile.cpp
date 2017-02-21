#include "tile.h"
#include "tile_sprite_state.h"

void Tile::Create(int x, int y, int tileType)
{
	GameEntity::Create();

	horizontalPosition = x;
	verticalPosition = y;

	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TileSpriteState::TILE_WIDTH;
	mBox.h = TileSpriteState::TILE_HEIGHT;

	//Get the tile type
	mType = tileType;

	currentStateType = tileType;

	size->w = TileSpriteState::TILE_WIDTH;
	size->h = TileSpriteState::TILE_HEIGHT;
}

void Tile::Update(float dt) {
	GameEntity::Update(dt);

	
}

int Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}