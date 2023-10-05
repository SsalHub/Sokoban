#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "UtilData.h"

#define _SCREEN_WIDTH_      120
#define _SCREEN_HEIGHT_     30
#define _MAP_WIDTH_         40
#define _MAP_HEIGHT_        20

typedef enum Flag
{
	_TRUE_ = 0,
	_FALSE_,
	_BLOCKED_,
	_STAGE_CLEAR_,
	_STAGE_RESTART_,
	_STAGE_SELECT_,
	_START_GAME_,
	_EXIT_GAME_,
} Flag;

typedef enum GameObject
{
	_NONE_ 			= 0,
	_PLAYER_ 		= 1,
	_WALL_			= 2,
	_BALL_			= 3,
	_EMPTY_BOX_		= 4,
	_FILLED_BOX_	= 5,
	_OUT_OF_MAP_    = 6,
} GameObject;

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
	GameObject map[_MAP_HEIGHT_][_MAP_WIDTH_];
	GameObject structure[_MAP_HEIGHT_][_MAP_WIDTH_];
	Position playerBeginPos;
	Position originalBoxesPos[_MAP_WIDTH_*_MAP_HEIGHT_];
} MapData;

typedef struct MapDataDoublyLinkedList
{
	struct MapDataDoublyLinkedList* before;
	struct MapDataDoublyLinkedList* after;
	MapData mapData;
} MapDataDLL;

extern char character[7][5];        // HEIGHT of array : GameObjects, WIDTH of array : char bytes(unicode)
extern Position player;
extern MapData mapData;
extern MapDataDLL* head;
extern MapDataDLL* tail;

void fortestfunc(char*);

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
MapData* findMapData(int);
