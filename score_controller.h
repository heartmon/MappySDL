#pragma once
#include "game_entity.h"
#include "score.h"
#include "item.h"

class ScoreController : public GameEntity {
	ObjectPool<Score> scores_pool;
	SDL_Rect* camera;
	AvancezLib* system;

public:
	void Create(AvancezLib* system, SDL_Rect* camera) {
		this->camera = camera;
		this->system = system;
		scores_pool.Create(20);
	}

	void Update(float dt) {
		for (auto go = scores_pool.pool.begin(); go != scores_pool.pool.end(); go++) {
			(*go)->Update(dt);
		}
	}

	void MakeScoreFrom(GameEntity* gameEntity) {
		Score * score = scores_pool.FirstAvailable();

		if (gameEntity->getName() == CLASS_ITEM) {
			Item* item = (Item*)gameEntity;
			score->Create(system, item->horizontalPosition, item->verticalPosition - 25, Score::SCORE_FROM_ITEM, item->getBaseScore());

			DrawTextRenderComponent* drawComponent = new DrawTextRenderComponent();
			drawComponent->Create(system, score, camera);
			score->AddComponent(drawComponent);

			CameraCollideComponent* cameraComp = new CameraCollideComponent();
			cameraComp->Create(system, score, nullptr, camera);
			score->AddComponent(cameraComp);

			score->Init();

			this->Send(new Message(UPDATE_SCORE, this, item->getBaseScore()));
		}
	}

	void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == COLLECT_ITEM) {
			SDL_Log("Get score!!");
			MakeScoreFrom(m->getArg1());
		}
	}
};