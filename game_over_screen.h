#pragma once
#include "game_entity.h"
#include "avancezlib.h"

class GameOverScreen : public GameEntity {
	AvancezLib* system;
public:
	virtual void Create(AvancezLib* system) {
		this->system = system;
	}

	virtual void Update(float dt)
	{
		//AvancezLib::KeyStatus keys;
		//system->getKeyStatus(keys);

		//if (keys.enter) {
		//	enabled = false;
		//	system->getRouter()->setCurrentScreen(PLAY_SCREEN);
		//}
	}

	virtual void Draw()
	{
		char msg[1024];
		sprintf(msg, "GAME OVER :(");
		system->drawText(250, 8, msg);
	}
};