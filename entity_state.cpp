#include "entity_state.h"

EntityState::EntityState(SpriteSheet* spriteSheet, int stateType, int numberOfFrame) {
	this->spriteSheet = spriteSheet;
	this->stateType = stateType;
	this->numberOfFrame = numberOfFrame;
}

EntityState::EntityState(SpriteSheet* spriteSheet, int stateType, int numberOfFrame, SDL_Rect* clipingRect) {
	this->spriteSheet = spriteSheet;
	this->stateType = stateType;
	this->numberOfFrame = numberOfFrame;
	this->clipingRect = clipingRect;

	SDL_Log("%d", this->clipingRect->x);
}

void EntityState::setClipingRect(SDL_Rect* clipingRect) {
	this->clipingRect = clipingRect;
}

int EntityState::getStateType() {
	return stateType;
}

SDL_Rect* EntityState::getClipingRect() {
	return clipingRect;
}

int EntityState::getNumberOfFrame() {
	return numberOfFrame;
}

void EntityState::Render(float x, float y, int frame) {
	//SDL_Log("%f", clipingRect->h);
	//SDL_Rect* test = new SDL_Rect();
	//test->x = 0;
	//test->y = 0;
	//test->w = 32;
	//test->h = 32;
	spriteSheet->render(x, y, clipingRect);
}