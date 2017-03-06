#pragma once
#include "component.h"
#include "message.h"
#include "global_constant.h"

class ItemBehaviorComponent : public Component {

public:
	virtual void Create(AvancezLib* system, GameEntity* go, std::vector<GameEntity*> * gameObjects) {
		Component::Create(system, go, gameObjects);
	}

	virtual void Init() {
		
	}

	void Update(float dt) {

	}

private:

};