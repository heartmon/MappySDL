#pragma once
#include "component.h"

class MouseBehaviorComponentInterface : public Component {
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects) = 0;
	//virtual void Update(float dt) = 0;
	//virtual void Receive(int message) {}
	//virtual void Destroy() {}
private:
};
