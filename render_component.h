#pragma once
#include "avancezlib.h"
#include "game_entity.h"

template <class T>
class RenderComponent {
public:
	virtual ~RenderComponent() {};
	virtual void Update(float dt, T* entity) = 0;
private:
	
};