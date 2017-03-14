#pragma once
#include "game_entity.h"
#include "avancezlib.h"
#include "score_total.h"

class Info : public GameEntity {
	AvancezLib* system;
	int mappyLife;
	int level;
	int scoreTotal;
	//ScoreTotal* scoreTotal;

public:
	virtual void Create(AvancezLib* system) {
		this->system = system;
		
	/*	scoreTotal = new ScoreTotal();
		scoreTotal->Create();
		*/
	}

	void Init() {
		GameEntity::Init();
		SDL_Log("Info::Init");

		scoreTotal = 0;

	}

	void Update(float dt) {
		Draw();
	}

	void Draw() {
		system->SetRendererViewport(GameViewportType::INFO_VIEWPORT);

		SDL_Color blue = { 0,0,255 };
		SDL_Color red = { 255,0,0 };

		// Static text
		char scoreTitle[10];
		sprintf(scoreTitle, "SCORE");
		system->drawText(150, 8, scoreTitle, red, false);

		char lifeTitle[10];
		sprintf(lifeTitle, "MAPPY");
		system->drawText(300, 8, lifeTitle, blue, false);

		char levelTitle[10];
		sprintf(levelTitle, "LEVEL");
		system->drawText(450, 8, levelTitle, blue, false);

		// Dynamic
		char scoreText[20], mouseLifeText[5], levelText[5];
		sprintf(scoreText, "%d", scoreTotal);
		system->drawText(150, 30, scoreText);

		sprintf(mouseLifeText, "%d", mappyLife);
		system->drawText(300, 30, mouseLifeText);

		sprintf(levelText, "%d", level);
		system->drawText(450, 30, levelText);
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == UPDATE_LIFE) {
			mappyLife = m->getData();
		}
		if (m->getMessageType() == UPDATE_LEVEL) {
			level = m->getData();
		}
		if (m->getMessageType() == UPDATE_SCORE) {
			scoreTotal += m->getData();
		}
	}
};