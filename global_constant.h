#pragma once
#include <string>
//Screen dimension constants
const int SCREEN_WIDTH = 560;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 480;

const int TILE_ROWS = 32;
const int TILES_COLUMNS = 12;
const int TOTAL_TILES = TILE_ROWS * TILES_COLUMNS;


//Tile constants
//const int TILE_WIDTH = 24;
//const int TILE_HEIGHT = 24;
//const int TOTAL_TILES = 600;
//const int TOTAL_TILE_SPRITES = 6;

/* Registered class name */
const std::string CLASS_MOUSE = "Mouse";
const std::string CLASS_TILE = "Tile";
const std::string CLASS_GAME = "Game";
const std::string CLASS_LEVEL = "Level";
const std::string CLASS_ROPE = "Rope";