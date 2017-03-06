#pragma once
#include <string>
#include "SDL.h"

class GameEntity;
enum MessageType { 
	HIT, 
	MOUSE_JUMP_TO_STAND, 
	TOGGLE_DOOR, 
	DOOR_OPEN, 
	DOOR_CLOSE, 
	GAME_OVER, 
	RELEASE_RAINBOW, 
	LEVEL_WIN, 
	NO_MSG, 
	TILE_MAP_HIT,
	HEAD_HIT,
	UPDATE_LIFE,
	UPDATE_LEVEL,
	UPDATE_SCORE,
	COLLECT_ITEM,
	MOUSE_DIE,
	LIFE_DECREASE
};

class Message {
	MessageType messageType;
	GameEntity* entity1;
	GameEntity* entity2;
	int data;

public:
	Message(MessageType messageType, GameEntity* entity1 = nullptr, int data = 0) {
		this->messageType = messageType;
		this->entity1 = entity1;
		this->data = data;
	}
	~Message() { 

	}

	void Destroy() {
		delete this;
	}


	MessageType getMessageType() {
		return messageType;
	}

	GameEntity* getArg1() {
		return entity1;
	}

	int getData() {
		return data;
	}

	/*GameEntity* getArg2() {
		return entity2;
	}*/

private:
};