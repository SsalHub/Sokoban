#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "UtilData.h"

#define _SCREEN_WIDTH_      	120
#define _SCREEN_HEIGHT_     	30
#define _MAP_WIDTH_         	40
#define _MAP_HEIGHT_        	20
#define _CURRENT_MAP_INDEX_		0
#define _ORIGIN_MAP_INDEX_		1

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

typedef struct MapData
{
	int stageIndex;
	int width;
	int height;
	int boxCount;
	GameObject map[2][_MAP_HEIGHT_][_MAP_WIDTH_];
	COORD playerBeginPos;
	COORD currPlayerPos;
	COORD originalBoxesPos[_MAP_WIDTH_*_MAP_HEIGHT_];
} MapData;

typedef struct MapDataDoublyLinkedList
{
	struct MapDataDoublyLinkedList* before;
	struct MapDataDoublyLinkedList* after;
	MapData mapData;
} MapDataDLL;

extern char character[7][5];        // HEIGHT of array : GameObjects, WIDTH of array : char bytes(unicode)
extern MapDataDLL* head;
extern MapDataDLL* tail;

void fortestfunc(char*);

void initGame();
void exitGame();
/* Load and Read game files, and Initialize game datas about stage map data. */
int countMaxStage();
void loadMapData(MapData*, int);
void releaseMapDataDLL();
MapDataDLL* findMapDataDLL(int);
void copyMapData(MapData*, MapData*);
/* Control playing stage and player. */
void setPlayerPos(MapData*, int, int);
Flag translatePlayerPos(MapData*, int, int);
Flag pushBall(MapData*, int, int);
Flag pushFilledBox(MapData*, int, int);
void changePositionState(MapData*, int, int, GameObject);
bool EqualsWithPlayerPos(MapData*, int, int);
bool checkClearStage(MapData*);
