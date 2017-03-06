#pragma once
#include "game_entity.h"
#include "global_constant.h"

class Router : public GameEntity {
	ScreenName currentScreen;
public:
	void setCurrentScreen(ScreenName newScreen) {
		currentScreen = newScreen;
	}

	ScreenName getCurrentScreen() {
		return currentScreen;
	}
};