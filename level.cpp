#define _CRT_SECURE_NO_WARNINGS
#include "level.h"
#include "global_constant.h"
#include <fstream>
#include "tile.h"
#include "tile_sprite_state.h"

// Message of complete level -> change level

void Level::Create(AvancezLib* system, SDL_Rect* camera) {
	this->system = system;
	this->level = 1;

	tss = new TileSpriteState();
	tss->Create(system);

	this->camera = camera;

	this->tileMap = new std::vector<Tile*>;
}

void Level::Init() {
	GameEntity::Init();
	//set tile map based on current level value (1 ~ ...)
	this->SetTileMap();
	/*for (int i = 0; i < TOTAL_TILES; ++i) {
		tileSet[i]->Init();
	}*/
	for (std::vector<Tile*>::iterator it = tileMap->begin(); it != tileMap->end(); ++it) {
		Tile* tile = *it;
		tile->Init();
	}
}

void Level::Update(float dt) {
	GameEntity::Update(dt);

	/*for (int i = 0; i < TOTAL_TILES; ++i) {
		tileSet[i]->Update(dt);
	}*/
	for (std::vector<Tile*>::iterator it = tileMap->begin(); it != tileMap->end(); ++it) {
		Tile* tile = *it;
		tile->Update(dt);
	}

	//SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	////Render level
	//for (int i = 0; i < TOTAL_TILES; ++i)
	//{
	//	//tileSet[i]->render(camera);
	//	//SDL_Rect mBox = tileSet[i]->getBox();
	//	int mType = tileSet[i]->getType();
	//	if (checkCollision(camera, tileSet[i]->getBox()))
	//	{
	//		//SDL_Log("Render ready! %d", tileClips[mType].x);
	//		//Show the tile
	//		tileSprite->render(tileSet[i]->getBox().x - camera.x, tileSet[i]->getBox().y - camera.y, &tileClips[mType]);
	//	}
	//}
}

void Level::SetTileMap() {
	SDL_Log("Level %d .. setting tile map", level);

	//Choose the path base on current level
	char levelPath[20];
	sprintf(levelPath, "data/level%d.map", level);

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
		if ((tileType >= 0) && (tileType < TileSpriteState::TOTAL_TILE_SPRITES))
		{
			Tile* tile = new Tile();
			tile->Create(x, y, tileType);
			// set component to each tile
			SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
			spriteSheetRenderComponent->Create(system, tile, NULL, tss, true, camera);
			CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
			cameraCollideComponent->Create(system, tile, NULL, camera);
			tile->AddComponent(spriteSheetRenderComponent);
			tile->AddComponent(cameraCollideComponent);

			tileMap->push_back(tile);

			//tileSet[i] = new Tile();
			//tileSet[i]->Create(x, y, tileType);
			//// set component to each tile
			//SpriteSheetRenderComponent* spriteSheetRenderComponent = new SpriteSheetRenderComponent();
			//spriteSheetRenderComponent->Create(system, tileSet[i], NULL, tss, true, camera);
			//CameraCollideComponent* cameraCollideComponent = new CameraCollideComponent();
			//cameraCollideComponent->Create(system, tileSet[i], NULL, camera);
			//tileSet[i]->AddComponent(spriteSheetRenderComponent);
			//tileSet[i]->AddComponent(cameraCollideComponent);
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
}