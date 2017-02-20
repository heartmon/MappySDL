#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <set>
#include "component.h"

class MouseBehaviorComponent : public Component {
public:
	void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects);
	void Update(float dt);
	void Receive(int message);
	void Destroy();
	void Move(float move);
	struct KeyStatus {
		bool left; // left arrow
		bool right; // right arrow
	};
private:
	KeyStatus key;

};