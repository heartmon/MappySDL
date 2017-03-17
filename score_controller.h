#pragma once
#include "game_entity.h"
#include "score.h"
#include "item.h"
#include <cmath>

class ScoreController : public GameEntity {
	ObjectPool<Score> scores_pool;
	SDL_Rect* camera;
	AvancezLib* system;

	int scorePowerBase = 2;
	int scoreMultiplier = 100;
	int bigcatScore = 800;
public:
	void Create(AvancezLib* system, SDL_Rect* camera) {
		this->camera = camera;
		this->system = system;
		scores_pool.Create(20);
		for (auto go = scores_pool.pool.begin(); go != scores_pool.pool.end(); go++) {
			Score* score = (*go);
			score->Create(system, camera);

			DrawTextRenderComponent* drawComponent = new DrawTextRenderComponent();
			drawComponent->Create(system, score, camera);
			score->AddComponent(drawComponent);

			CameraCollideComponent* cameraComp = new CameraCollideComponent();
			cameraComp->Create(system, score, nullptr, camera);
			score->AddComponent(cameraComp);
		}
	}

	void Init() {
		GameEntity::Init();
		SDL_Log("ScoreController:: Init");
	}


	void Update(float dt) {
		for (auto go = scores_pool.pool.begin(); go != scores_pool.pool.end(); go++) {
			(*go)->Update(dt);
		}
	}

	void MakeScoreFrom(GameEntity* gameEntity) {
		Score * scoreEntity = scores_pool.FirstAvailable();
		if (scoreEntity == NULL) {
			return;
		}
		if (gameEntity->getName() == CLASS_ITEM) {
			Item* item = (Item*)gameEntity;

			int score = item->getBaseScore();
			int multiplier = 1;
			if (item->isBonus) {
				multiplier = 2;
			}
			
			scoreEntity->Init(item->horizontalPosition, item->verticalPosition - 25, Score::SCORE_FROM_ITEM, item->getBaseScore(), multiplier);

			this->Send(new Message(UPDATE_SCORE, scoreEntity, score*multiplier));
		}
	}

	void MakeRainbowScore(int score, int multiplier = 1) {
		Score * scoreEntity = scores_pool.FirstAvailable();
		if (scoreEntity == NULL) {
			return;
		}
		//scoreEntity->Init(SCREEN_WIDTH / 2 - 50, 0, Score::SCORE_FROM_POWER, score);
		scoreEntity->Init(0, SCREEN_HEIGHT / 2, Score::SCORE_FROM_POWER, score, multiplier);

		this->Send(new Message(UPDATE_SCORE, scoreEntity, score*multiplier));
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == IS_COLLECTED) {
			SDL_Log("Get score!!");
			MakeScoreFrom(m->getArg1());
		}

		if (m->getMessageType() == MOUSE_JUMP_ON_ROPE) {
			this->Send(new Message(UPDATE_SCORE, this, 10));
		}

		if (m->getMessageType() == RAINBOW_GONE) {
			RainbowBehaviorComponent* behaviorComponent = (RainbowBehaviorComponent*)m->getArg1()->getBehaviorComponent();
			SDL_Log("Cat die : %d, Big cat die : %d", behaviorComponent->numberOfCats, behaviorComponent->numberOfBigCats);
			int catScore = pow(scorePowerBase, behaviorComponent->numberOfCats)*scoreMultiplier;
			if (behaviorComponent->numberOfCats == 0) {
				catScore = 0;
			}
			int bigCatScore = bigcatScore * behaviorComponent->numberOfBigCats;
			int multiplier = behaviorComponent->numberOfBigCats * 2;

			int totalScore = catScore + bigCatScore;
			if (totalScore > 0) {
				MakeRainbowScore(totalScore, multiplier);
			}
		}
	}
};