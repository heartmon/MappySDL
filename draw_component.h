#pragma once
#include "avancezlib.h"
#include "game_entity.h"

template <class T>
class DrawComponent {
public:
	virtual ~DrawComponent() {};
	virtual void Update(int dt, T* entity) = 0;
private:

};