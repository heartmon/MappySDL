#pragma once
#include <vector>
#include <string>
#include "message.h"
#include "global_constant.h"

class CollisionRuleInterface;
class EntityState;
class Component;
class GameEntity {
protected:
	// observer pattern
	std::vector<GameEntity*> receivers;
	std::vector<Component*> components;

public:
	// describes an axis-aligned rectangle with a velocity
	struct Box
	{
		// position of top-left corner
		float x, y;

		// dimensions
		float w, h;

		// velocity
		float vx, vy;
	};
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
	virtual void Receive(Message* m);
	void Send(Message* m);

	int getCurrentStateType();
	void setCurrentStateType(int currentStateType);

	float horizontalPosition;
	float verticalPosition;
	int direction; //left = -1, right = 1;
	bool enabled;
	bool isVisibleWithinCamera;
	bool isXCollidedWithMap;
	bool isYCollidedWithMap;
	int animationFrame = 0;
	float arbitaryTrackingNumber = 0;
	const int static LEFT = -1;
	const int static RIGHT = 1;

	float vy = 0;
	float vx = 0;
	float ax = 1;
	float ay = 1;
	float w, h;

	virtual GameEntity::Box getCollisionBox(SDL_Rect* camera = NULL) {
		int x = horizontalPosition;
		int y = verticalPosition;
		if (camera) {
			x += camera->x;
			y += camera->y;
		}
		//SDL_Rect a = { x, y, size->w, size->h };
		//return a;

		GameEntity::Box collisionBox;
		collisionBox.x = x;
		collisionBox.y = y;
		collisionBox.w = getSize()->w;
		collisionBox.h = getSize()->h;
		collisionBox.vx = vx;
		collisionBox.vy = vy;

		return collisionBox;
	}
protected:
	int currentStateType = -1;
	CollisionRuleInterface* collisionRule;
	SDL_Rect* size;
	
private:
};