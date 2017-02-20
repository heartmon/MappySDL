#pragma once
#include <set>
#include "entity_state.h"

class SpriteStateInterface {
public:
	//virtual ~StateSetInterface();
	virtual void Create(AvancezLib* system) = 0;
	virtual std::set<EntityState*>* getSpriteStateEntities() = 0;
protected:
	std::set<EntityState*>* states;
private:
	
};