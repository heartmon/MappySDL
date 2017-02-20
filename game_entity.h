#pragma once
#include <vector>

enum Message { HIT, ALIEN_HIT, GAME_OVER, LEVEL_WIN, NO_MSG };

class EntityState;
class Component;
class GameEntity {
protected:
	// observer pattern
	std::vector<GameEntity*> receivers;
	std::vector<Component*> components;
public:
	~GameEntity();
	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Update(float dt);
	virtual void Init();
	virtual void Destroy();
	virtual void AddReceiver(GameEntity *ge);
	virtual void Receive(Message m) {}
	void Send(Message m);

	int getCurrentStateType();
	void setCurrentStateType(int currentStateType);

	float horizontalPosition;
	float verticalPosition;
	int direction; //left = -1, right = 1;
	bool enabled;
	int animationFrame = 0;

	const int static LEFT = -1;
	const int static RIGHT = 1;
protected:
	int currentStateType = -1;
private:
};