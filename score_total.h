#pragma once
#include "game_entity.h"

class ScoreTotal : public GameEntity {
	int score;
public:
	void Init() {
		score = 0;
	}

	void Update(float dt) {

	}

	void Draw() {

	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

	}

	int getScore() {
		return score;
	}
};