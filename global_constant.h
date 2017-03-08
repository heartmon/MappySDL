#pragma once
#include <string>
//Screen dimension constants
const int SCREEN_WIDTH = 660;
const int SCREEN_HEIGHT = 480;

const int WINDOW_WIDTH = 660;
const int WINDOW_HEIGHT = 580;

const int X_SCREEN_OFFSET = 0;
const int Y_SCREEN_OFFSET = WINDOW_HEIGHT - SCREEN_HEIGHT;

//The dimensions of the level
const int LEVEL_WIDTH = 1200;
const int LEVEL_HEIGHT = 480;

const int TILE_ROWS = 30;
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
const std::string CLASS_DOOR = "Door";
const std::string CLASS_RAINBOW = "Rainbow";
const std::string CLASS_ITEM = "Item";
const std::string CLASS_CAT = "Cat";
const std::string CLASS_BIG_CAT = "BigCat";

enum ScreenName { START_SCREEN, PLAY_SCREEN, GAME_OVER_SCREEN };
