#pragma once
#include "component.h"
#include "message.h"
#include "global_constant.h"

class Item;
class ItemBehaviorComponent : public Component {
	Item* gameEntity;
public:
	virtual void Create(AvancezLib* system, Item* go, std::vector<GameEntity*> * gameObjects);
	virtual void Init();
	void Update(float dt);

private:

};