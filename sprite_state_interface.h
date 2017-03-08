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
	SpriteSheet* getSpriteSheet() {
		return spriteSheet;
	}
	virtual int getSpriteWidth() = 0;
	virtual int getSpriteHeight() = 0;

	virtual void Destroy() {
	}

protected:
	std::vector<EntityState*>* states;
	SpriteSheet* spriteSheet;

	void InitClip(int state, int y, int animationFrame, SDL_Rect* clip) {
		for (int i = 0; i < animationFrame; i++) {
			clip[i].x = getSpriteWidth() * (i + state);
			clip[i].y = y;
			clip[i].w = getSpriteWidth();
			clip[i].h = getSpriteHeight();
		}
	}

	SDL_Rect* Create(int state, int y, int animationFrame) {
		SDL_Rect* clip;
		for (int i = 0; i < animationFrame; i++) {
			clip[i].x = getSpriteWidth() * (i + state);
			clip[i].y = y;
			clip[i].w = getSpriteWidth();
			clip[i].h = getSpriteHeight();
		}
		return clip;
	}

	EntityState* CreateEntityState(int state, int frame, SDL_Rect* clip, int animationSpeed = 10) {
		return new EntityState(spriteSheet, state, frame, clip, animationSpeed);
	}
private:
	
	
};