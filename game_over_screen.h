#pragma once
#include "game_entity.h"
#include "avancezlib.h"

class GameOverScreen : public GameEntity {
	AvancezLib* system;
	float timeCount = 0;
	float waitingTimeInSec = 6;

	int score;
public:
	virtual void Create(AvancezLib* system) {
		this->system = system;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		timeCount += dt;
		if (timeCount > waitingTimeInSec) {
			timeCount = 0;
			enabled = false;
			system->getRouter()->setCurrentScreen(START_SCREEN);
		}
	}

	virtual void Draw()
	{
		char msg[1024];
		SDL_Color red = { 255,0,0 };
		sprintf(msg, "GAME OVER :(");

		char hs[50];
		sprintf(hs, "SCORE     %d", score);

		system->SetRendererViewport(FULL_VIEWPORT);

		system->drawText(225, WINDOW_HEIGHT / 2 - 35, msg, red, true);
		system->drawText(225, WINDOW_HEIGHT / 2 + 15, hs, red, true);
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == HIGH_SCORE) {
			score = m->getData();
		}
	}
};