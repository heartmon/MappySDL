//#pragma once
//#include "render_component.h"
//#include "avancezlib.h"
//#include <vector>
//
//class Mouse;
//class MouseRenderComponent : public RenderComponent<Mouse> {
//	//Constants
//	const static int STANDING_ANIMATION_FRAME = 1;
//	const static int WALKING_ANIMATION_FRAME = 2;
//	const static int WALKING_ANIMATION_INTERVAL = 0.5f;
//	const static int PREJUMP_ANIMATION_FRAME = 1;
//	const static int JUMP_ANIMATION_FRAME = 2;
//
//	const int TILE_PATH = 2;
//	const int TILE_CORNER_RIGHT = 3;
//	const int TILE_SPACE = 4;
//	const int TILE_BASE = 5;
//
//	SDL_Rect standSpriteClips[STANDING_ANIMATION_FRAME];
//	SDL_Rect walkSpriteClips[WALKING_ANIMATION_FRAME];
//	SDL_Rect prejumpSpriteClips[PREJUMP_ANIMATION_FRAME];
//	SDL_Rect jumpSpriteClips[JUMP_ANIMATION_FRAME];
//
//	//Prop
//	AvancezLib* system;
//	//Sprite* sprite;
//	SpriteSheet* sprite;
//	std::vector<SDL_Rect*> spriteClips;
//	int frame = 0;
//
//public:
//	MouseRenderComponent(AvancezLib* system);
//	void Update(float dt, Mouse* entity);
//private:
//	SDL_Rect* getSpriteClips(int entityState);
//	int getAnimationFrame(int entityState);
//	int previousState;
//};
