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
	//SDL_Rect tileClips[6];
	int level = 1;

	//SpriteSheetRenderComponent* spriteSheetRenderComponent;
	//CameraCollideComponent* cameraCollideComponent;

	TileSpriteState* tss;
public:
	void Create(AvancezLib* system, SDL_Rect* camera);
	void Init();
	void Update(float dt);
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
	void SetTileMap();
};
