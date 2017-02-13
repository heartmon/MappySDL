#pragma once
#include "mouse_render_component.h"
#include "mouse.h"

MouseRenderComponent::MouseRenderComponent(AvancezLib* system) {
	this->system = system;

	//setup variable
	//spriteClips.reserve(5);
	int spriteSize = 32;

	//setup sprite
	sprite = system->createSpriteSheet("data/mappy_sprite.png");
	SDL_Log("%d", sprite->getWidth());
	SDL_Log("%d", sprite->getHeight());

	//setup clipping rect
	//stand animation
	standSpriteClips[0].x = spriteSize * 1;
	standSpriteClips[0].y = 0;
	standSpriteClips[0].w = spriteSize;
	standSpriteClips[0].h = spriteSize;

	//walking animation
	walkSpriteClips[0].x = spriteSize * 0;
	walkSpriteClips[0].y = 0;
	walkSpriteClips[0].w = spriteSize;
	walkSpriteClips[0].h = spriteSize;

	walkSpriteClips[1].x = spriteSize * 1;
	walkSpriteClips[1].y = 0;
	walkSpriteClips[1].w = spriteSize;
	walkSpriteClips[1].h = spriteSize;

	//spriteClips.push_back(standSpriteClips);
	//spriteClips.push_back(walkSpriteClips);
}

SDL_Rect* MouseRenderComponent::getSpriteClips(int entityState) {
	switch (entityState) {
	case Mouse::STATE_STAND:
		return standSpriteClips;
		break;
	case Mouse::STATE_WALK:
		return walkSpriteClips;
		break;
	default:
		return standSpriteClips;
	}
}

int MouseRenderComponent::getAnimationFrame(int entityState) {
	switch (entityState) {
	case Mouse::STATE_STAND:
		return STANDING_ANIMATION_FRAME;
		break;
	case Mouse::STATE_WALK:
		return WALKING_ANIMATION_FRAME;
		break;
	default:
		return STANDING_ANIMATION_FRAME;
	}
}

void MouseRenderComponent::Update(float dt, Mouse* entity) {
	float t0 = system->getElapsedTime();
	int currentState = entity->getCurrentState();
	SDL_Rect* currentClip = getSpriteClips(currentState);
	//SDL_Log("%d, %d, %d, %d", currentClip->x, currentClip->y, currentClip->w, currentClip->h);
	int frame;
	if (currentState != previousState) {
		frame = 0;
	}
	else {
		frame = ((int)(t0*10) % getAnimationFrame(currentState));
	}
		
	// move
	if (currentState == Mouse::STATE_WALK) {
		entity->horizontalPosition = entity->horizontalPosition + (dt * 160.0f) * entity->direction;
	}

	// jump

	sprite->render(entity->horizontalPosition, entity->verticalPosition, &currentClip[frame]);
	previousState = currentState;
}
