#pragma once
#include "SDL.h"
#include "global_constant.h"

enum GameViewportType { INFO_VIEWPORT, MAIN_VIEWPORT, FULL_VIEWPORT };

class GameViewport {
	SDL_Rect* mainViewport;
	SDL_Rect* infoViewport;
	SDL_Rect* fullViewport;
public:
	

	void Init() {
		// Setup viewport
		mainViewport = new SDL_Rect();
		//MainViewport
		mainViewport->x = 0;
		mainViewport->y = Y_SCREEN_OFFSET;
		mainViewport->w = SCREEN_WIDTH;
		mainViewport->h = SCREEN_HEIGHT;
		//SDL_RenderSetViewport(system->getRenderer(), &playScreenViewport);

		infoViewport = new SDL_Rect();
		infoViewport->x = 0;
		infoViewport->y = 0;
		infoViewport->w = SCREEN_WIDTH;
		infoViewport->h = Y_SCREEN_OFFSET;

		fullViewport = new SDL_Rect();
		fullViewport->x = 0;
		fullViewport->y = 0;
		fullViewport->w = SCREEN_WIDTH;
		fullViewport->h = SCREEN_HEIGHT;
	}

	SDL_Rect* getViewport(GameViewportType type) {
		switch (type) {
		case INFO_VIEWPORT:
			return infoViewport;
			break;
		case MAIN_VIEWPORT:
			return mainViewport;
			break;
		case FULL_VIEWPORT:
			return fullViewport;
			break;
		default:
			return nullptr;
		}
	}

	//static SDL_Rect* getMainViewport() {
	//	return mainViewport;
	//}

	//static SDL_Rect* getInfoViewport() {
	//	return infoViewport;
	//}
};