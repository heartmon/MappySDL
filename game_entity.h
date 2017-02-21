#pragma once
#include <vector>
#include <string>
#include "message.h"
#include "collision_rule_interface.h"
#include "global_constant.h"

class EntityState;
class Component;
class GameEntity {
protected:
	// observer pattern
	std::vector<GameEntity*> receivers;
	std::vector<Component*> components;

public:
	~GameEntity();
	virtual std::string getName() {
		return "";
	}

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual SDL_Rect* getSize() {
		return size;
	}

	void SetCollisionRule(CollisionRuleInterface* collisionRule);
	CollisionRuleInterface* getCollisionRule() {
		return collisionRule;
	}

	virtual void Update(float dt);
	virtual void Init();
	virtual void Destroy();
	virtual void AddReceiver(GameEntity *ge);
	virtual void Receive(Message* m) {
		// Check HIT = collided
		if (m->getMessageType() == HIT) {
			collisionRule->didHit(m);
		}
	}
	void Send(Message* m);

	int getCurrentStateType();
	void setCurrentStateType(int currentStateType);

	float horizontalPosition;
	float verticalPosition;
	int direction; //left = -1, right = 1;
	bool enabled;
	bool isVisibleWithinCamera;
	bool isCollidedWithMap;
	int animationFrame = 0;

	const int static LEFT = -1;
	const int static RIGHT = 1;
protected:
	int currentStateType = -1;
	CollisionRuleInterface* collisionRule;
	SDL_Rect* size;
private:
};