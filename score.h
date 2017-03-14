#pragma once
#include "draw_entity.h"

class Score : public DrawEntity {
	int scoreType;
	int score;
	AvancezLib* system;
	SDL_Rect* camera;
	std::string scoreText;

	float scoreDisplayTime = 0;
	float scoreDisplayInterval = 2.f;
	float scoreSpeedY = -20.f;
	float scoreCreditSpeedY = 160.f;
	float scoreSpeedX = 350.f;
public:
	const static int SCORE_FROM_ITEM = 0;
	const static int SCORE_FROM_POWER = 1;
	int multiplier = 1;

	virtual void Create(AvancezLib* system, SDL_Rect* camera) {
		DrawEntity::Create();
		this->system = system;
		this->camera = camera;

		//SDL_Log("Score Created at:: %f, %f with score %d", horizontalPosition, verticalPosition, score);		

		
	}

	virtual void Init(int x, int y, int scoreType, int score, int multiplier = 1) {
		GameEntity::Init();
		this->horizontalPosition = x;
		this->verticalPosition = y;
		this->scoreType = scoreType;
		this->score = score;
		this->multiplier = multiplier;
		size->w = 50;
		size->h = 30;
	}

	virtual SDL_Color getColor() {
		return { 255, 255, 255 };
	}

	virtual std::string getMsg() {
		char buff[20];
		if (multiplier > 1) {
			sprintf(buff, "%d x %d", score, multiplier);
			scoreText = buff;
		}
		else {
			sprintf(buff, "%d", score);
			scoreText = buff;
		}
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
		switch (scoreType) {
			case SCORE_FROM_ITEM:
				if (scoreDisplayTime > scoreDisplayInterval) {
					scoreDisplayTime = 0;
					enabled = false;
				}
				break;
			case SCORE_FROM_POWER:
				//verticalPosition += dt*scoreCreditSpeedY;
				horizontalPosition += dt*scoreSpeedX;
				if (verticalPosition > SCREEN_HEIGHT) {
					scoreDisplayTime = 0;
					enabled = false;
				}
				if (horizontalPosition > LEVEL_WIDTH) {
					scoreDisplayTime = 0;
					enabled = false;
				}
				break;
		}

	}

	bool isLargeFont() {
		if (scoreType == SCORE_FROM_POWER) {
			return true;
		}
		else {
			return false;
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