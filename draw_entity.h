#pragma once
#include "game_entity.h"

class DrawEntity : public GameEntity {
protected:
	//SDL_Color drawColor;
	//char* drawMsg;
public:

	virtual void Create() {
		GameEntity::Create();
	}

	virtual SDL_Color getColor() = 0;
	virtual std::string getMsg() = 0;


};