#define _CRT_SECURE_NO_WARNINGS
#include "level.h"
#include "global_constant.h"
#include <fstream>
#include "tile.h"
#include "tile_sprite_state.h"
#include "rope.h"
#include "rope_collision_rule.h"
#include "item.h"
#include "door.h"
#include "door_collision_rule.h"
#include "item_collision_rule.h"
// Message of complete level -> change level

void Level::Create(AvancezLib* system, SDL_Rect* camera) {
	SDL_Log("Level::Create");
	this->system = system;
	this->level = 1;


	this->camera = camera;

	this->tileMap = new std::vector<Tile*>;
	this->ropeArray = new std::vector<Rope*>;
	this->itemArray = new std::vector<Item*>;
	this->doorArray = new std::vector<Door*>;


	// set common variables
	tss = new TileSpriteState();
	tss->Create(system);

	itemSpriteState = new ItemSpriteState();
	itemSpriteState->Create(system);

	doorSpriteState = new DoorSpriteState();
	doorSpriteState->Create(system);

	//ropeSpriteState = new RopeSpriteState();
	//ropeSpriteState->Create(system);

	// setup pools
	doorPool.Create(20);
	for (auto d = doorPool.pool.begin(); d != doorPool.pool.end(); d++) {
		Door* door = *d;
		SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
		spriteSheetRenderComponent->Create(system, door, NULL, doorSpriteState, true, camera);
		CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
		cameraCollideComponent->Create(system, door, NULL, camera);
		DoorBehaviorComponent* doorBehaviorComponent = new DoorBehaviorComponent();
		doorBehaviorComponent->Create(system, door, doorArray, camera);
		DoorCollisionRule* doorCollisionRule = new DoorCollisionRule();
		doorCollisionRule->Create(door, camera, doorBehaviorComponent);

		door->Create();
		door->SetCollisionRule(doorCollisionRule);
		door->AddComponent(spriteSheetRenderComponent);
		door->AddComponent(cameraCollideComponent);
		door->AddBehaviorComponent(doorBehaviorComponent);
	}

	//
	itemPool.Create(15);
	for (auto i = itemPool.pool.begin(); i != itemPool.pool.end(); i++) {
		Item* item = *i;

		SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
		spriteSheetRenderComponent->Create(system, item, NULL, itemSpriteState, true, camera);

		CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
		cameraCollideComponent->Create(system, item, NULL, camera);

		ItemBehaviorComponent* itemBehaviorComponent = new ItemBehaviorComponent();
		itemBehaviorComponent->Create(system, item, nullptr);

		ItemCollisionRule* itemCollisionRule = new ItemCollisionRule();
		itemCollisionRule->Create(item, camera, itemBehaviorComponent);

		item->Create();
		item->SetCollisionRule(itemCollisionRule);
		item->AddComponent(spriteSheetRenderComponent);
		item->AddComponent(cameraCollideComponent);
		item->AddBehaviorComponent(itemBehaviorComponent);
	}

	//
	ropePool.Create(10);
	for (auto r = ropePool.pool.begin(); r != ropePool.pool.end(); r++) {
		Rope* rope = *r;

		RopeSpriteState* ropeSpriteState = new RopeSpriteState();
		ropeSpriteState->Create(system);

		SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
		spriteSheetRenderComponent->Create(system, rope, NULL, ropeSpriteState, true, camera);

		CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
		cameraCollideComponent->Create(system, rope, NULL, camera);

		RopeBehaviorComponent* ropeBehaviorComponent = new RopeBehaviorComponent();
		ropeBehaviorComponent->Create(system, rope, nullptr, ropeSpriteState);

		RopeCollisionRule* ropeCollisionRule = new RopeCollisionRule();
		ropeCollisionRule->Create(rope, camera, ropeBehaviorComponent);

		rope->Create();
		rope->SetCollisionRule(ropeCollisionRule);
		rope->AddComponent(spriteSheetRenderComponent);
		rope->AddComponent(cameraCollideComponent);
		rope->AddBehaviorComponent(ropeBehaviorComponent);
		
	}

	//tilemap
	tileMapPool.Create(TILE_ROWS*TILES_COLUMNS + 10);
	for (auto it = tileMapPool.pool.begin(); it != tileMapPool.pool.end(); it++) {
		Tile *tile = *it;

		SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
		spriteSheetRenderComponent->Create(system, tile, NULL, tss, true, camera);
		CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
		cameraCollideComponent->Create(system, tile, NULL, camera);

		tile->Create();
		tile->AddComponent(spriteSheetRenderComponent);
		tile->AddComponent(cameraCollideComponent);

	}
}

void Level::Init(int level) {
	GameEntity::Init();
	SDL_Log("Level::Init");

	//set tile map based on current level value (1 ~ ...)
	this->SetTileMap(level);
	/*for (int i = 0; i < TOTAL_TILES; ++i) {
		tileSet[i]->Init();
	}*/
	/*for (std::vector<Tile*>::iterator it = tileMap->begin(); it != tileMap->end(); ++it) {
		Tile* tile = *it;
		tile->Init();
	}

	for (std::vector<Rope*>::iterator it = ropeArray->begin(); it != ropeArray->end(); ++it) {
		Rope* rope = *it;
		rope->Init();
	}

	for (std::vector<Item*>::iterator it = itemArray->begin(); it != itemArray->end(); ++it) {
		Item* item = *it;
		item->Init();
	}

	for (std::vector<Door*>::iterator it = doorArray->begin(); it != doorArray->end(); ++it) {
		Door* door = *it;
		door->Init();
	}*/
}

void Level::Update(float dt) {
	GameEntity::Update(dt);

	for (auto it = tileMapPool.pool.begin(); it != tileMapPool.pool.end(); it++) {
		Tile* tile = *it;
		tile->Update(dt);
	}

	for (auto it = ropePool.pool.begin(); it != ropePool.pool.end(); it++) {
		Rope* rope = *it;
		rope->Update(dt);
	}

	for (auto it = itemPool.pool.begin(); it != itemPool.pool.end(); it++) {
		Item* item = *it;
		item->Update(dt);
	}

	for (auto it = doorPool.pool.begin(); it != doorPool.pool.end(); it++) {
		Door* door = *it;
		door->Update(dt);
	}
}

void Level::RoundInit(int level) {

}

void Level::SetTileMap(int level) {
	// clean everyting up
	for (auto it = tileMapPool.pool.begin(); it != tileMapPool.pool.end(); it++) {
		(*it)->enabled = false;
	}

	for (auto it = ropePool.pool.begin(); it != ropePool.pool.end(); it++) {
		(*it)->enabled = false;
	}

	for (auto it = itemPool.pool.begin(); it != itemPool.pool.end(); it++) {
		(*it)->enabled = false;
	}

	for (auto it = doorPool.pool.begin(); it != doorPool.pool.end(); it++) {
		(*it)->enabled = false;
	}


	SDL_Log("Level %d .. setting tile map", level);

	//Choose the path base on current level
	char levelPath[20];
	sprintf(levelPath, "data/level%d.txt", level);

	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map(levelPath);


	//Initialize the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//Determines what kind of tile will be made
		int tileType = -1;

		//Read tile from map file
		map >> tileType;

		//If the was a problem in reading the map
		if (map.fail())
		{
			//Stop loading map
			printf("Error loading map: Unexpected end of file!\n");
			tilesLoaded = false;
			break;
		}


		//If the number is a valid tile number
		//SDL_Log("TileType:: %d", tileType);
		if ((tileType >= 0))
		{
			int isDoorType = true;
			Door* door = doorPool.FirstAvailable();
			int doorOffsetY = -27;
			switch (tileType) {
				case TileSpriteState::STATE_TILE_DOOR_LEFT:
					door->Init((float)x - 26, (float)y + doorOffsetY, 0);
					break;
				case TileSpriteState::STATE_TILE_DOOR_RIGHT:
					door->Init((float)x + 3, (float)y + doorOffsetY, 1);
					break;
				case TileSpriteState::STATE_TILE_DOOR_POWER_LEFT:
					door->Init((float)x - 26, (float)y + doorOffsetY, 2);
					break;
				case TileSpriteState::STATE_TILE_DOOR_POWER_RIGHT:
					door->Init((float)x + 3, (float)y + doorOffsetY, 3);
					break;
				default:
					isDoorType = false;
			}

			switch (tileType) {
				case  TileSpriteState::STATE_TILE_ITEM_100:
				case  TileSpriteState::STATE_TILE_ITEM_200:
				case  TileSpriteState::STATE_TILE_ITEM_300:
				case  TileSpriteState::STATE_TILE_ITEM_400:
				case  TileSpriteState::STATE_TILE_ITEM_500:
					SDL_Log("Item is being created?");
					Item* item = itemPool.FirstAvailable();
					item->Init((float)x, (float)y, tileType - 10 - 1, (tileType - 10)*100);
					break;
			}

			// special adding
			if (tileType == TileSpriteState::STATE_TILE_ROPE) {
				SDL_Log("Rope is being created?");
				Rope* rope = ropePool.FirstAvailable();
				rope->Init((float)x, (float)y - RopeSpriteState::SPRITE_HEIGHT/2 + 5);
			}

			Tile* tile = tileMapPool.FirstAvailable();
			tile->Init(x, y, tileType, i);
		}

		//If we don't recognize the tile type
		else
		{
			//Stop loading map
			printf("Error loading map: Invalid tile type at %d!\n", i);
			tilesLoaded = false;
			break;
		}

		//Move to next tile spot
		x += TileSpriteState::TILE_WIDTH;


		//If we've gone too far
		if (x >= LEVEL_WIDTH)
		{
			//Move back
			x = 0;

			//Move to the next row
			y += TileSpriteState::TILE_HEIGHT;
		}
	}

	map.close();
}