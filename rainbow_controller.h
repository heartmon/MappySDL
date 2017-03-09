#pragma once
#include "game_entity.h"
#include "rainbow.h"
#include "door.h"
#include "rainbow_sprite_state.h"
#include "rainbow_behavior_component.h"

class RainbowController : public GameEntity {
	std::vector<Rainbow*> rainbows;
	SDL_Rect* camera;
	AvancezLib* system;
	RainbowSpriteState* spriteState;

public:
	void Create(AvancezLib* system, SDL_Rect* camera) {
		GameEntity::Create();
		this->camera = camera;
		this->system = system;
		//rainbows = new std::vector<Rainbow*>();

		spriteState = new RainbowSpriteState();
		spriteState->Create(system);
	}

	void Init() {
		GameEntity::Init();
		SDL_Log("RainbowController::Init");
	}

	void Update(float dt) {
		for (auto go = rainbows.begin(); go != rainbows.end(); go++) {
			(*go)->Update(dt);
		}
	}

	void MakeRainbowFrom(Door* door) {
		SDL_Log("Rainbow is creating !!");
		SDL_Log("With door %f, %f", door->horizontalPosition, door->verticalPosition);
		//rainbow will start from the door position and move toward the direction
		int direction = 0;
		if (door->defaultState == DoorSpriteState::STATE_DOOR_POWER_LEFT) {
			direction = GameEntity::LEFT;
		}
		if (door->defaultState == DoorSpriteState::STATE_DOOR_POWER_RIGHT) {
			direction = GameEntity::RIGHT;
		}
		if(direction == 0) {
			SDL_Log("Something wrong with rainbow direction..");
			return;
		}

		Rainbow* rainbow = new Rainbow();
		rainbow->Create(door->horizontalPosition, door->verticalPosition, direction);

		CameraCollideComponent* cameraComponent = new CameraCollideComponent();
		cameraComponent->Create(system, rainbow, nullptr, camera);
		SpriteSheetRenderComponent* spriteComponent = new SpriteSheetRenderComponent();
		spriteComponent->Create(system, rainbow, nullptr, spriteState, false, camera, true);
		RainbowBehaviorComponent* behaviorComponent = new RainbowBehaviorComponent();
		behaviorComponent->Create(system, rainbow, camera);

		rainbow->AddComponent(cameraComponent);
		rainbow->AddComponent(spriteComponent);
		rainbow->AddComponent(behaviorComponent);


		rainbow->Init();

		rainbows.push_back(rainbow);
	}

	virtual void Receive(Message* m) {
		GameEntity::Receive(m);
		if (m->getMessageType() == RELEASE_RAINBOW && m->getArg1()->getName() == CLASS_DOOR) {
			MakeRainbowFrom((Door*)m->getArg1());
		}
	}
private:


};