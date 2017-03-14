#pragma once
#include "item_behavior_component.h"
#include "item.h"

void ItemBehaviorComponent::Create(AvancezLib* system, Item* go, std::vector<GameEntity*> * gameObjects) {
	Component::Create(system, go, gameObjects);
	this->gameEntity = go;
}

void ItemBehaviorComponent::Init() {

}

void ItemBehaviorComponent::Update(float dt) {
	if (gameEntity->isBonus) {
		gameEntity->setCurrentStateType(gameEntity->bonusState);
	}
	else {
		gameEntity->setCurrentStateType(gameEntity->defaultState);
	}
}

