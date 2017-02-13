#pragma once
#include "game_entity.h"

template <class T>
class InputComponent {
public:
	virtual ~InputComponent() {}
	virtual void Update(float dt, T* entity) = 0;

};