#pragma once
#include <set>
#include <vector>
#include "object_pool.h"
#include "game_viewport.h"

class GameEntity;
class DrawEntity;
class AvancezLib;
class Sprite;
class SpriteSheet;
class EntityState;
class SpriteStateInterface;
class SDL_Rect;
class Tile;

class Component
{
protected:
	AvancezLib * system;
	GameEntity * gameEntity;	// the game object this component is part of
	std::vector<GameEntity*> * allGameEntities;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects);

	virtual void Init() {}
	virtual void RoundInit() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite;

public:

	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, const char * sprite_name);
	virtual void Update(float dt);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
};

class SpriteSheetRenderComponent : public Component {
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SpriteStateInterface* spriteState, bool order = false, SDL_Rect* camera = NULL, bool flip = false, GameViewportType viewportType = MAIN_VIEWPORT);
	virtual void Update(float dt);
	virtual void Destroy();

private:
	SpriteStateInterface* spriteState;
	bool order = false;
	SDL_Rect* camera;
	bool flip ;
	GameViewportType viewportType;
};

class DrawTextRenderComponent : public Component {
public:
	virtual void Create(AvancezLib* system, DrawEntity * go, SDL_Rect* camera = NULL, GameViewportType viewportType = MAIN_VIEWPORT);
	virtual void Update(float dt);
	virtual void Destroy();
private:
	DrawEntity* drawEntity;
	SDL_Rect* camera;
	GameViewportType viewportType;
};

class CameraCollideComponent : public Component {
	SDL_Rect* camera;
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera);
	virtual void Update(float dt);
	virtual void Destroy();
private:
};

class MapCollideComponent : public Component {
	SDL_Rect* camera;
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, std::vector<Tile*>* tileMap);
	virtual void Update(float dt);
	virtual void Destroy();
private:
	std::vector<Tile*>* tileMap;
};



class CollidePoolComponent : public Component
{
	ObjectPool<GameEntity> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, ObjectPool<GameEntity> * coll_objects);
	virtual void Update(float dt);
};

class CollideComponent : public Component 
{
	std::vector<GameEntity*>* collObjects;
public:
	virtual void Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, std::vector<GameEntity*> * collObjects);
	virtual void Update(float dt);
};

