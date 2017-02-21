#pragma once

class Message;
class GameEntity;
class CollisionRuleInterface {
public:
	virtual void Create(GameEntity* gameEntity) {
		this->gameEntity = gameEntity;
	};
	virtual void didHit(Message* message) {};
	virtual int isCollided(GameEntity* withThisEntity) { 
		//auto -> use default of component
		return -1; 
		// return 0 ; false
		// return 1 ; true
	};
protected:
	GameEntity* gameEntity;
private:
};