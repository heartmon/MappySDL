#pragma once
#include "SDL.h"
#include "avancezlib.h"
// Contain each GameEntity's state and sprites associated with
class SpriteSheet;
class EntityState {
public:
	EntityState(SpriteSheet* spriteSheet, int stateType, int numberOfFrame);
	EntityState(SpriteSheet* spriteSheet, int stateType, int numberOfFrame, SDL_Rect* clipingRect);
	EntityState(SpriteSheet* spriteSheet, int stateType, int numberOfFrame, SDL_Rect* clipingRect, int animationSpeed, bool noRepeat = false);
	void setClipingRect(SDL_Rect* clipingRect);
	int getStateType();
	SDL_Rect* getClipingRect();
	int getNumberOfFrame();

	float frameInterval = 0.5f;

	void Render(float x, float y, int frame, bool flip = false);

	int getAnimationSpeed() {
		return animationSpeed;
	}
	bool isNoRepeat() {
		return noRepeat;
	}
private:
	SpriteSheet* spriteSheet;
	//array of frames
	SDL_Rect* clipingRect;
	int stateType = -1;
	int numberOfFrame;
	int animationSpeed;
	bool noRepeat;
};