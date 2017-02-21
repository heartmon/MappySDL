#pragma once
#include <string>
#include "SDL.h"

enum MessageType { HIT, ALIEN_HIT, GAME_OVER, LEVEL_WIN, NO_MSG, TILE_MAP_HIT };

class Message {
	MessageType messageType;
	std::string arg1;
	std::string arg2;
public:
	Message(MessageType messageType, const char* arg1 = "", const char* arg2 = "") {
		this->messageType = messageType;
		this->arg1 = arg1;
		this->arg2 = arg2;
	}
	~Message() { 

	}

	void Destroy() {
		delete this;
	}


	MessageType getMessageType() {
		return messageType;
	}

	std::string getArg1() {
		return arg1;
	}

	std::string getArg2() {
		return arg2;
	}

private:
};