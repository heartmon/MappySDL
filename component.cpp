#include "component.h"
#include "game_entity.h"
#include "avancezlib.h"
#include "entity_state.h"
#include "sprite_state_interface.h"
#include "tile.h"
#include "collision_rule_interface.h"

void Component::Create(AvancezLib * system, GameEntity * go, std::vector<GameEntity*>* allGameEntities)
{
	this->gameEntity = go;
	this->system = system;
	this->allGameEntities = allGameEntities;
}

void RenderComponent::Create(AvancezLib * system, GameEntity * go, std::vector<GameEntity*>* game_objects, const char * sprite_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!gameEntity->enabled)
		return;

	if (sprite)
		sprite->draw(int(gameEntity->horizontalPosition), int(gameEntity->verticalPosition));
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}


void CollideComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, ObjectPool<GameEntity> * coll_objects)
{
	Component::Create(system, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameEntity * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > gameEntity->horizontalPosition - 10) &&
				(go0->horizontalPosition < gameEntity->horizontalPosition + 10) &&
				(go0->verticalPosition   > gameEntity->verticalPosition - 10) &&
				(go0->verticalPosition   < gameEntity->verticalPosition + 10))
			{
				gameEntity->Receive(new Message(HIT));
				go0->Receive(new Message(HIT));
			}
		}
	}
}

void SpriteSheetRenderComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SpriteStateInterface* spriteState, bool order, SDL_Rect* camera ) {
	Component::Create(system, go, game_objects);
	this->spriteState = spriteState;
	this->order = order;
	this->camera = camera;
}
void SpriteSheetRenderComponent::Update(float dt) {
	if (!gameEntity->isVisibleWithinCamera) {
		return;
	}

	int offsetCameraX = 0;
	int offsetCameraY = 0;

	if (camera != NULL) {
		offsetCameraX = camera->x;
		offsetCameraY = camera->y;
	}

	// 1 check the state
	if (order) {
		std::vector<EntityState*>* states = spriteState->getSpriteStateEntities();
		int currentState = gameEntity->getCurrentStateType();
		EntityState* s = states->at(currentState);

		if (s->getStateType() != currentState) {
			SDL_Log("Mismatch state order");
		}

		// Update animation frame
		float t0 = system->getElapsedTime();
		int frame = gameEntity->animationFrame;
		frame = ((int)(t0 * 10) % s->getNumberOfFrame());
		gameEntity->animationFrame = frame;

		//Render
		s->Render(gameEntity->horizontalPosition - offsetCameraX, gameEntity->verticalPosition - offsetCameraY, frame);
	}
	else {
		std::vector<EntityState*>* states = spriteState->getSpriteStateEntities();
		int num = 0;
		for (std::vector<EntityState*>::iterator it = states->begin(); it != states->end(); ++it) {
			EntityState* s = *it;
			SDL_Rect* rect = s->getClipingRect();

			// check current state for using in render
			if (s->getStateType() == gameEntity->getCurrentStateType()) {

				// Update animation frame
				float t0 = system->getElapsedTime();
				int frame = gameEntity->animationFrame;
				frame = ((int)(t0 * 10) % s->getNumberOfFrame());

				gameEntity->animationFrame = frame;

				// Render using the spritesheet (SpriteSheet.render)
				s->Render(gameEntity->horizontalPosition, gameEntity->verticalPosition, frame);
			}
		}
	}
}
void SpriteSheetRenderComponent::Destroy() {

}

void CameraCollideComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, SDL_Rect* camera) {
	Component::Create(system, go, game_objects);
	this->camera = camera;
}
void CameraCollideComponent::Update(float dt) {
	// check if the game entity is within camera area
	SDL_Rect* camera = this->camera;
	
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = camera->x;
	rightA = camera->x + camera->w;
	topA = camera->y;
	bottomA = camera->y + camera->h;

	//Calculate the sides of rect B
	leftB = gameEntity->horizontalPosition;
	rightB = gameEntity->horizontalPosition + 48;
	topB = gameEntity->verticalPosition;
	bottomB = gameEntity->verticalPosition + 48;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		gameEntity->isVisibleWithinCamera = false;
	}
	else if (topA >= bottomB)
	{
		gameEntity->isVisibleWithinCamera = false;
	}
	else if (rightA <= leftB)
	{
		gameEntity->isVisibleWithinCamera = false;
	}
	else if (leftA >= rightB)
	{
		gameEntity->isVisibleWithinCamera = false;
	}
	else {
		//If none of the sides from A are outside B
		gameEntity->isVisibleWithinCamera = true;
	}

	
}
void CameraCollideComponent::Destroy() {

}

void MapCollideComponent::Create(AvancezLib* system, GameEntity * go, std::vector<GameEntity*> * game_objects, std::vector<Tile*>* tileMap) {
	Component::Create(system, go, game_objects);
	this->tileMap = tileMap;
}
void MapCollideComponent::Update(float dt) {
	int count = 0;
	for (std::vector<Tile*>::iterator it = tileMap->begin(); it != tileMap->end(); ++it) {
		Tile* tile = *it;
		//std::vector<GameEntity*>* extraEntities = new std::vector<GameEntity*>();
		//if (count + TILE_ROWS < tileMap->size()) {
		//	extraEntities->push_back(tileMap->at(count));
		//}
		//

		if (tile->enabled) {
			int collidedResult = gameEntity->getCollisionRule()->isCollided(tile, dt, tileMap);
			if (collidedResult == -1) {
				if ((tile->horizontalPosition > gameEntity->horizontalPosition - 10) &&
					(tile->horizontalPosition < gameEntity->horizontalPosition + 10) &&
					(tile->verticalPosition > gameEntity->verticalPosition - 10) &&
					(tile->verticalPosition < gameEntity->verticalPosition + 10))
				{
					collidedResult = 1;
				}
			}

			if (collidedResult == 1) {
				Message* m = new Message(HIT, tile->getName().c_str());
				gameEntity->Receive(m);
				//go0->Receive(new Message(HIT));
				m->Destroy();

				//gameEntity->isCollidedWithMap = true;
			}
			else {
				//gameEntity->isCollidedWithMap = false;
			}
		}
		count++;
		//delete extraEntities;
	}
	//for (auto i = 0; i < sizeof(tileMap) / sizeof(tileMap[0]); i++)
	//{
	//	GameEntity * go0 = coll_objects->pool[i];
	//	if (go0->enabled)
	//	{
	//		if ((go0->horizontalPosition > gameEntity->horizontalPosition - 10) &&
	//			(go0->horizontalPosition < gameEntity->horizontalPosition + 10) &&
	//			(go0->verticalPosition   > gameEntity->verticalPosition - 10) &&
	//			(go0->verticalPosition   < gameEntity->verticalPosition + 10))
	//		{
	//			gameEntity->Receive(new Message(HIT));
	//			go0->Receive(new Message(HIT));
	//		}
	//	}
	//}
}
void MapCollideComponent::Destroy() {

}