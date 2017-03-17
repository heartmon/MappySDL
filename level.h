#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include "component.h"
#include <vector>

class Rope;
class Tile;
class TileSpriteState;
class Item;
class Door;
class Roof;
class ItemSpriteState;
class DoorSpriteState;
class RopeSpriteState;
class RoofSpriteState;
class Level : public GameEntity {
	AvancezLib* system;
	SDL_Rect* camera;
	Tile* tileSet[TOTAL_TILES];

	ObjectPool<Item> itemPool;
	ObjectPool<Rope> ropePool;
	ObjectPool<Tile> tileMapPool;
	ObjectPool<Door> doorPool;
	ObjectPool<Roof> roofPool;

	TileSpriteState* tss;
	ItemSpriteState* itemSpriteState;
	DoorSpriteState* doorSpriteState;
	RoofSpriteState* roofSpriteState;
	int roofStartOffset = 25;

public:
	void Create(AvancezLib* system, SDL_Rect* camera);
	void Init(int level);
	void Update(float dt);
	void RoundInit(int level);

	ObjectPool<Tile>* getTileMapPool() {
		return &tileMapPool;
	}
	ObjectPool<Rope>* getRopePool() {
		return &ropePool;
	}
	ObjectPool<Item>* getItemPool() {
		return &itemPool;
	}
	ObjectPool<Door>* getDoorPool() {
		return &doorPool;
	}
	ObjectPool<Roof>* getRoofPool() {
		return &roofPool;
	}
private:
	void SetTileMap(int level);
};
