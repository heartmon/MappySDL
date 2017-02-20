#pragma once
#include "mouse_behavior_component_interface.h"
#include "avancezlib.h"

class MouseBehaviorComponent : public MouseBehaviorComponentInterface  {
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