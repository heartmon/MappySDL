#pragma once
#include <vector>
#include "entity_state.h"

class SpriteStateInterface {
public:
	const int static FRAME_NO_MOVE = 1;
	//virtual ~StateSetInterface();
	virtual void Create(AvancezLib* system) = 0;
	std::vector<EntityState*>* getSpriteStateEntities() {
		return states;
	}
protected:
	std::vector<EntityState*>* states;
private:
	
};