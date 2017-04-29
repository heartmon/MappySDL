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
	IS_COLLECTED,
	MOUSE_DIE,
	LIFE_DECREASE,
	LEVEL_CLEAR,
	CAT_DIE,
	RAINBOW_GONE,
	POSSESS_ITEM,
	GET_BIG_CAT_SCORE,
	GAME_INIT,
	MOUSE_JUMP_ON_ROPE,
	ROUND_INIT,
	HIGH_SCORE,
	MOUSE_TOGGLE_DOOR
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