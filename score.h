#pragma once
#include "draw_entity.h"

class Score : public DrawEntity {
	int scoreType;
	int score;
	AvancezLib* system;
	std::string scoreText;

	float scoreDisplayTime = 0;
	float scoreDisplayInterval = 1.f;
	float scoreSpeedY = -20.f;
public:
	const static int SCORE_FROM_ITEM = 0;
	const static int SCORE_FROM_POWER = 1;

	virtual void Create(AvancezLib* system, int x, int y, int scoreType, int score) {
		DrawEntity::Create();
		this->system = system;
		this->horizontalPosition = x;
		this->verticalPosition = y;
		this->scoreType = scoreType;
		this->score = score;
		SDL_Log("Score Created at:: %f, %f with score %d", horizontalPosition, verticalPosition, score);		

		size->w = 50;
		size->h = 30;
	}

	virtual SDL_Color getColor() {
		return { 255, 255, 255 };
	}

	virtual std::string getMsg() {
		char buff[20];
		sprintf(buff, "%d", score);
		scoreText = buff;
		//SDL_Log("XXXXXXXXXXXX %s", scoreText);
		return scoreText;
	}

	virtual void Update(float dt) {
		DrawEntity::Update(dt);

		if (!enabled) {
			return;
		}
		
		scoreDisplayTime += dt;
		//verticalPosition += dt*scoreSpeedY;

		if (scoreDisplayTime > scoreDisplayInterval) {
			scoreDisplayTime = 0;
			enabled = false;
		}

	}

	void Draw() {
		/*system->SetRendererViewport(MAIN_VIEWPORT);
		char scoreText[20];
		sprintf(scoreText, "%d", score);*/
		//SDL_Log("%d", scoreText);
		//system->drawText(horizontalPosition, verticalPosition, scoreText);
	}

	
};