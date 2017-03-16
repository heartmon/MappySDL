#pragma once
#include "game_entity.h"
#include "avancezlib.h"

class StartScreen : public GameEntity {
	AvancezLib* system;
	Sprite* startScreenSprite;
	int highscore;
public:
	virtual void Create(AvancezLib* system) {
		this->system = system;

		startScreenSprite = system->createSpriteNonBmp("data/start_screen.png");
	}

	//virtual void Init()
	//{

	//}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);

		if (keys.enter) {
			enabled = false;
			system->getRouter()->setCurrentScreen(PLAY_SCREEN);
		}

		if (enabled) {
			startScreenSprite->draw(WINDOW_WIDTH/2 - 375, 0);
		}
	}

	virtual void Draw()
	{
		char msg[50];
		int sec = SDL_GetTicks() / 1000;

		sprintf(msg, "Please enter to start");

		char hs[50];
		sprintf(hs, "HIGH SCORE      %d", highscore);

		SDL_Color red = { 255,0,0 };
		system->SetRendererViewport(FULL_VIEWPORT);

		if (sec % 2 == 0) {
			system->drawText(225, WINDOW_HEIGHT / 2 - 10, msg, red, false);
		}
		system->drawText(225, WINDOW_HEIGHT / 2 + 20, hs, red, false);
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == HIGH_SCORE) {
			highscore = m->getData();
		}
	}
};