#pragma once
#include "avancezlib.h"
#include "game_entity.h"
#include <vector>
#include "component.h"

class MouseBehaviorComponent : public Component {
	SDL_Rect* camera;
	float cameraHorizontalFloat;
	float cameraVerticalFloat;
public:
	void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera);
	void Update(float dt);
	void Receive(int message);
	void Destroy();
	void Move(float move);

private:
	void MoveCamera();

};