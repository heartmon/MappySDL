#pragma once
#include "global_constant.h"
#include "avancezlib.h"
#include "game_entity.h"

class Tile : public GameEntity {
public:
	//Get the tile type
	int getType();
	//Get the collision box
	SDL_Rect getBox();
	void Create(int x, int y, int tileType);
	void Update(float dt);

	virtual std::string getName() {
		return CLASS_TILE;
	}
private:
	//The attributes of the tile
	SDL_Rect mBox;
	//The tile type
	int mType;
};