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
	unsigned int id;
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
	virtual ~GameEntity();
	virtual std::string getName() {
		return "";
	}
	virtual unsigned int getId() {
		return id;
	}

	virtual void Create();
	virtual void Create(float x, float y) {
		Create();
		horizontalPosition = x;
		verticalPosition = y;
	}
	virtual void AddComponent(Component * component);
	virtual void AddBehaviorComponent(Component * component);
	virtual SDL_Rect* getSize() {
		if (size->w == 0) SDL_Log("*********** %s::Not specified size->w and size->h", getName());
		return size;
	}

	void SetCollisionRule(CollisionRuleInterface* collisionRule);
	CollisionRuleInterface* getCollisionRule() {
		return collisionRule;
	}

	virtual void Update(float dt);
	virtual void Init();
	virtual void Init(float x, float y);
	virtual void RoundInit();
	virtual void Destroy();
	virtual void AddReceiver(GameEntity *ge);
	virtual void Receive(Message* m);
	void Send(Message* m);
	virtual void ClearReceivers();

	virtual int getCurrentStateType();
	virtual void setCurrentStateType(int currentStateType);
	virtual int getWidth() { return 0; };
	virtual int getHeight() { return 0; };

	float horizontalPosition;
	float verticalPosition;
	int direction; //left = -1, right = 1;
	bool enabled;
	bool isVisibleWithinCamera;
	bool isXCollidedWithMap;
	bool isYCollidedWithMap;
	bool isStop;
	int animationFrame = 0;
	float arbitaryTrackingNumber = 0.f;
	const int static LEFT = -1;
	const int static RIGHT = 1;

	float vy = 0.f;
	float vx = 0.f;
	float ax = 1.f;
	float ay = 1.f;
	float w, h;

	virtual GameEntity::Box getCollisionBox(SDL_Rect* camera = NULL) {
		int x = (int)horizontalPosition;
		int y = (int)verticalPosition;
		if (camera) {
			x += camera->x;
			y += camera->y;
		}

		GameEntity::Box collisionBox;
		collisionBox.x = x;
		collisionBox.y = y;
		collisionBox.w = getSize()->w;
		collisionBox.h = getSize()->h;
		collisionBox.vx = vx;
		collisionBox.vy = vy;

		return collisionBox;
	}

	virtual Component* getBehaviorComponent() {
		return behaviorComponent;
	}
protected:
	int currentStateType = -1;
	CollisionRuleInterface* collisionRule;
	SDL_Rect* size;
	Component* behaviorComponent;
	
private:
};