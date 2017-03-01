#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include "component.h"
#include <vector>

class Rope;
class Tile;
class TileSpriteState;

class Level : public GameEntity {
	//The different tile sprites

	//private prop
	AvancezLib* system;
	SDL_Rect* camera;
	//SpriteSheet* tileSprite;
	Tile* tileSet[TOTAL_TILES];
	
	std::vector<Rope*>* ropeArray;
	std::vector<Tile*>* tileMap;
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
private:
	void SetTileMap();
};
