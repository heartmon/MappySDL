#pragma once
#include <vector>
#include "entity_state.h"

class SpriteStateInterface {
public:
	//virtual ~StateSetInterface();
	virtual void Create(AvancezLib* system) = 0;
	virtual std::vector<EntityState*>* getSpriteStateEntities() = 0;
	virtual EntityState* getMatchEntityState(int currentStateType) = 0;
protected:
	std::vector<EntityState*>* states;
private:
	
};