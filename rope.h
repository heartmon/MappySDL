#pragma once
#include "tile.h"

class Rope : public Tile {
public:
	//virtual void Create(int x, int y, int tileType) {	}
	virtual void Update(float dt) {

	}

	virtual std::string getName() {
		return CLASS_ROPE;
	}
private:

};