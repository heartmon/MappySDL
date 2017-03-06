#include "game_entity.h"
#include "component.h"
#include "avancezlib.h"
#include "entity_state.h"
#include "collision_rule_interface.h"

void GameEntity::Create()
{
	//SDL_Log("GameEntity::Create");

	enabled = false;
	isVisibleWithinCamera = false;
	isXCollidedWithMap = false;
	isYCollidedWithMap = false;
	direction = 0;
	size = new SDL_Rect;
	setCurrentStateType(0);
}

void GameEntity::AddComponent(Component * component)
{
	components.push_back(component);
}

void GameEntity::AddBehaviorComponent(Component * component)
{
	AddComponent(component);
	behaviorComponent = component;
}

void GameEntity::SetCollisionRule(CollisionRuleInterface* collisionRule) {
	this->collisionRule = collisionRule;
}

void GameEntity::Init()
{
	//SDL_Log("GameObject::Init");

	for (auto it = components.begin(); it != components.end(); it++)
		(*it)->Init();

	enabled = true;
	isVisibleWithinCamera = true;

	ay = 1;
}

void GameEntity::RoundInit() {
	for (auto it = components.begin(); it != components.end(); it++) {
		(*it)->RoundInit();
	}
}

void GameEntity::Update(float dt)
{
	if (!enabled)
		return;

	for (auto it = components.begin(); it != components.end(); it++)
		(*it)->Update(dt);
}

void GameEntity::Destroy()
{
	for (auto it = components.begin(); it != components.end(); it++)
		(*it)->Destroy();
}

GameEntity::~GameEntity()
{
	SDL_Log("GameObject::~GameObject");
}

void GameEntity::AddReceiver(GameEntity * go)
{
	receivers.push_back(go);
}

void GameEntity::Send(Message* m)
{
	for (auto i = 0; i < receivers.size(); i++)
	{
		if (!receivers[i]->enabled)
			continue;
		receivers[i]->Receive(m);
	}
}

int GameEntity::getCurrentStateType() {
	return this->currentStateType;
}

void GameEntity::setCurrentStateType(int currentStateType) {
	//SDL_Log("%d", currentStateType);
	this->animationFrame = 0;
	this->currentStateType = currentStateType;
}

void GameEntity::Receive(Message* m) {
	// Check HIT = collided
	if (m->getMessageType() == HIT) {
		collisionRule->didHit(m);
	}
}