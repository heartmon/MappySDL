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
class ItemSpriteState;
class DoorSpriteState;
class RopeSpriteState;
class Level : public GameEntity {
	//The different tile sprites

	//private prop
	AvancezLib* system;
	SDL_Rect* camera;
	//SpriteSheet* tileSprite;
	Tile* tileSet[TOTAL_TILES];
	
	std::vector<Item*>* itemArray;
	std::vector<Rope*>* ropeArray;
	std::vector<Tile*>* tileMap;
	std::vector<Door*>* doorArray;

	ObjectPool<Item> itemPool;
	ObjectPool<Rope> ropePool;
	ObjectPool<Tile> tileMapPool;
	ObjectPool<Door> doorPool;


	//SDL_Rect tileClips[6];
	int level = 1;

	//SpriteSheetRenderComponent* spriteSheetRenderComponent;
	//CameraCollideComponent* cameraCollideComponent;

	TileSpriteState* tss;
	ItemSpriteState* itemSpriteState;
	DoorSpriteState* doorSpriteState;
	//RopeSpriteState* ropeSpriteState;
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

	std::vector<Tile*>* getTileMap() {
		return tileMap;
	}
	std::vector<Rope*>* getRopeArray() {
		return ropeArray;
	}
	std::vector<Item*>* getItemArray() {
		return itemArray;
	}
	std::vector<Door*>* getDoorArray() {
		return doorArray;
	}
private:
	void SetTileMap(int level);
};
