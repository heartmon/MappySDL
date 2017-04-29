#include "mouse.h"
#include "mouse_behavior_component.h"

void Mouse::Receive(Message* m)
{
	MouseBehaviorComponent* b = (MouseBehaviorComponent*)behaviorComponent;
	GameEntity::Receive(m);

	if (m->getMessageType() == DOOR_OPEN) {
		b->WhenDoorOpen(m->getArg1());
	}
	if (m->getMessageType() == DOOR_CLOSE) {
		b->WhenDoorClose(m->getArg1());
	}
	if (m->getMessageType() == HEAD_HIT) {
		b->WhenHeadHit();
	}
	if (m->getMessageType() == MOUSE_DIE) {
		b->WhenDie();
		Send(m);
	}
	if (m->getMessageType() == LEVEL_CLEAR) {
		isStop = true;
		setCurrentStateType(MouseSpriteState::STATE_STAND);
	}
	if (m->getMessageType() == MOUSE_TOGGLE_DOOR) {
		behaviorComponent->Receive(m);
	}

}

void Mouse::RoundInit() {
	GameEntity::RoundInit();
	direction = GameEntity::LEFT;
}