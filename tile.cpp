#include "tile.h"
#include "tile_sprite_state.h"

void Tile::Create() {
	GameEntity::Create();
}

void Tile::Create(int x, int y, int tileType)
{
	GameEntity::Create();

	horizontalPosition = x;
	verticalPosition = y;

	currentStateType = tileType;

	size->w = TileSpriteState::TILE_WIDTH;
	size->h = TileSpriteState::TILE_HEIGHT;
}

void Tile::Init(float x, float y, int tileType, unsigned int id) {
	GameEntity::Init(x, y);

	currentStateType = tileType;

	size->w = TileSpriteState::TILE_WIDTH;
	size->h = TileSpriteState::TILE_HEIGHT;

	this->id = id;
}

void Tile::Update(float dt) {
	GameEntity::Update(dt);
}