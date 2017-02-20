#pragma once
#include <set>
#include "object_pool.h"

class GameEntity;
class AvancezLib;
class Sprite;
class SpriteSheet;
class EntityState;
class SpriteStateInterface;

class Component
{
protected:
	AvancezLib * system;
	GameEntity * gameEntity;	// the game object this component is part of
	std::set<GameEntity*> * allGameEntities;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite;

public:

	virtual void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects, const char * sprite_name);
	virtual void Update(float dt);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
};

class SpriteSheetRenderComponent : public Component {
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects, SpriteStateInterface* spriteState);
	virtual void Update(float dt);
	virtual void Destroy();

private:
	SpriteStateInterface* spriteState;
};


class CollideComponent : public Component
{
	ObjectPool<GameEntity> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects, ObjectPool<GameEntity> * coll_objects);
	virtual void Update(float dt);
};

