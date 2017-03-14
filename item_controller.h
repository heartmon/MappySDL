#pragma once
#pragma once
#include "game_entity.h"
#include "cat.h"
#include "tile_sprite_state.h"
#include "object_pool.h"
#include "cat_behavior_component.h"
#include "cat_collision_rule.h"
#include "level.h"

class ItemController : public GameEntity {
	AvancezLib* system;
	SDL_Rect* camera;
	//std::vector<Cat*> cats;
	ObjectPool<Item>* item_pool;
	Level* level;
	Mouse* mouse;

	int numberOfItems;
	int numberOfCollectedItems = 0;

public:
	void Create(AvancezLib* system, SDL_Rect* camera, Level* level, Mouse* mouse) {
		GameEntity::Create();
		this->system = system;
		this->camera = camera;
		this->level = level;
		this->mouse = mouse;
		this->level = level;

		this->item_pool = level->getItemPool();
		
	}


	void Init() {
		GameEntity::Init();
		SDL_Log("ItemController::Init");

		numberOfCollectedItems = 0;
		numberOfItems = level->getItemPool()->Count();
	}


	void Update(float dt) {

	}

	void Receive(Message* m) {
		GameEntity::Receive(m);

		if (m->getMessageType() == IS_COLLECTED) {
			Item* collectedItem = (Item*)m->getArg1();
			numberOfCollectedItems++;
			SDL_Log("Item collected:: %d / %d", numberOfCollectedItems, numberOfItems);
			
			//check all items to toggle bonus state
			for (auto it = item_pool->pool.begin(); it != item_pool->pool.end(); it++) {
				Item* item = (*it);
				if (!item->enabled) continue;
				item->isBonus = false;

				// found the same kind of item
				if (collectedItem->getCurrentStateType() == item->getCurrentStateType()) {
					item->isBonus = true;
				}
			}

			//Forward message
			Send(m);

			if (numberOfCollectedItems >= numberOfItems) {
				Send(new Message(LEVEL_CLEAR, this));
			}
		}

		if (m->getMessageType() == TOGGLE_DOOR) {
			Send(m);
		}

		if (m->getMessageType() == DOOR_OPEN || m->getMessageType() == DOOR_CLOSE) {
			Send(m);
		}
	}
};