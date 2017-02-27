#pragma once
#include "sprite_state_interface.h"

class RopeSpriteState : public SpriteStateInterface {
public:
	const static int STATE_ROPE_STATIC = 0;
	const static int STATE_ROPE_JUMP_ON = 1;
};