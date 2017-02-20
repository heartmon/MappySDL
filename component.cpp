#include "component.h"
#include "game_entity.h"
#include "avancezlib.h"
#include "entity_state.h"
#include "sprite_state_interface.h"

void Component::Create(AvancezLib * system, GameEntity * go, std::set<GameEntity*>* allGameEntities)
{
	this->gameEntity = go;
	this->system = system;
	this->allGameEntities = allGameEntities;
}

void RenderComponent::Create(AvancezLib * system, GameEntity * go, std::set<GameEntity*>* game_objects, const char * sprite_name)
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


void CollideComponent::Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects, ObjectPool<GameEntity> * coll_objects)
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
				gameEntity->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

void SpriteSheetRenderComponent::Create(AvancezLib* system, GameEntity * go, std::set<GameEntity*> * game_objects, SpriteStateInterface* spriteState) {
	Component::Create(system, go, game_objects);
	this->spriteState = spriteState;
}
void SpriteSheetRenderComponent::Update(float dt) {
	//int currentStateType = gameEntity->getCurrentStateType();
	//EntityState* s = spriteState->getMatchEntityState(currentStateType);
	//s->Render(gameEntity->horizontalPosition, gameEntity->verticalPosition, 0);

	// 1 check the state
	std::vector<EntityState*>* states = spriteState->getSpriteStateEntities();
	for (std::vector<EntityState*>::iterator it = states->begin(); it != states->end(); ++it) {
		EntityState* s = *it;
		SDL_Rect* rect = s->getClipingRect();

		// check current state for using in render
		if (s->getStateType() == gameEntity->getCurrentStateType()) {
			//SDL_Log("!!!");
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
void SpriteSheetRenderComponent::Destroy() {

}