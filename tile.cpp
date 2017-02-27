#include "tile.h"
#include "tile_sprite_state.h"

void Tile::Create(int x, int y, int tileType)
{
	GameEntity::Create();

	horizontalPosition = x;
	verticalPosition = y;

	currentStateType = tileType;

	size->w = TileSpriteState::TILE_WIDTH;
	size->h = TileSpriteState::TILE_HEIGHT;
}

void Tile::Update(float dt) {
	GameEntity::Update(dt);
}