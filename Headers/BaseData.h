#pragma once

#include <stdbool.h>
#include <Windows.h>

#define _SCREEN_WIDTH_ 120
#define _SCREEN_HEIGHT_ 30
#define _MAP_WIDTH_ 50
#define _MAP_HEIGHT_ 30

typedef enum ConsoleColor
{
	_BLACK_ 		= 0,
	_BLUE_			= 1,
	_DARK_GREEN_	= 2,
	_SKYBLUE_		= 3,
	_RED_			= 4,
	_DARK_PURPLE_	= 5,
	_YELLOW_		= 6,
	_BRIGHT_GRAY_	= 7,
	_GRAY_			= 8,
	_OCEAN_BLUE_	= 9,
	_GREEN_			= 10,
	_SKY_			= 11,
	_HOTPINK_		= 12,
	_PURPLE_		= 13,
	_LIGHT_YELLOW_	= 14,
	_WHITE_			= 15,
} ConsoleColor;

typedef enum Flag
{
	_TRUE_ = 0,
	_FALSE_,
	_BLOCKED_,
	_STAGE_CLEAR_,
	_STAGE_RESTART_,
	_STAGE_SELECT_,
} Flag;

typedef enum GameObject
{
	_NONE_ 			= 0,
	_PLAYER_ 		= 1,
	_BLOCK_			= 2,
	_BALL_			= 3,
	_EMPTY_BOX_		= 4,
	_FILLED_BOX_	= 5,
} GameObject;

typedef enum InputType
{
	_SPACE_ 			= 32,
	_ESCAPE_ 			= 27,
	_LEFT_ 				= 75,
	_RIGHT_ 			= 77,
	_UP_ 				= 72,
	_DOWN_ 				= 80,
	_UPPER_R_ 			= 82,
	_LOWER_R_ 			= 114,
	_CARRIGE_RETURN_ 	= 13,
} InputType;

typedef struct Position
{
	int x;
	int y;
} Position;

typedef struct MapData
{
	int stageIndex;
	int width;
	int height;
	int boxCount;
	int map[_MAP_HEIGHT_][_MAP_WIDTH_];
	int structure[_MAP_HEIGHT_][_MAP_WIDTH_];
	Position box[_MAP_WIDTH_*_MAP_HEIGHT_];
} MapData;

typedef struct MapDataDoublyLinkedList
{
	struct MapDataDoublyLinkedList* before;
	struct MapDataDoublyLinkedList* after;
	MapData mapData;
} MapDataDLL;

extern Position player;
extern MapData mapData;
extern MapDataDLL* head;
extern MapDataDLL* tail;

void fortestfunc();
void initGame();
void setPlayerPos(int, int);
Flag translatePlayerPos(int, int);
Flag pushBall(int, int);
Flag pushFilledBox(int, int);
void changePositionState(int, int, GameObject);
bool EqualsWithPlayerPos(int, int);
void exitGame();
bool checkClearStage();
int countMaxStage();
void loadMapData(MapData*, int);
void releaseMapDataDLL();
void setMapData(MapData*, int);
void cleanInputBuffer();
