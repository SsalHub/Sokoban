#pragma once
#include <stdbool.h>
#include <Windows.h>

#define _SCREEN_WIDTH_ 120
#define _SCREEN_HEIGHT_ 30
#define _MAP_WIDTH_ 50
#define _MAP_HEIGHT_ 30

typedef enum ConsoleColor
{
	_BLACK_ = 0,
	_BLUE_,
	_GREEN_,
	_SKYBLUE_,
	_RED_,
	_PINK_,
	_ORANGE_,
	_WHITE_,
	_GRAY_,
	_LIGHTBLUE_,
	_BRIGHTGREEN_,
	_SKY_,
	_BRIGHTRED_,
	_BRIGHTPINK_,
	_BRIGHTYELLOW_,
	_BRIGHTWHITE_,
} ConsoleColor;

typedef enum Flag
{
	_TRUE_ = 0,
	_FALSE_,
	_BLOCKED_,
	_STAGE_CLEAR_,
} Flag;

typedef enum GameObject
{
	_NONE_ 			= 0,
	_PLAYER_ 		= 1,
	_BLOCK_			= 2,
	_BALL_			= 3,
	_EMPTY_BOX_	= 4,
	_FILLED_BOX_	= 5,
} GameObject;

typedef enum InputType
{
	_SPACE_ = 32,
	_ESCAPE_ = 27,
	_LEFT_ = 75,
	_RIGHT_ = 77,
	_UP_ = 72,
	_DOWN_ = 80,
} InputType;

typedef struct Position
{
	int x;
	int y;
} Position;

typedef struct MapData
{
	int width;
	int height;
	int houseCount;
	int map[_MAP_HEIGHT_][_MAP_WIDTH_];
	int structure[_MAP_HEIGHT_][_MAP_WIDTH_];
	Position house[_MAP_WIDTH_ * _MAP_HEIGHT_];
} MapData;

extern int currentScreenBufferIndex;
extern HANDLE stageScreenBuffer[2];
extern HANDLE effectBuffer;
extern HANDLE loadingStageBuffer[2];
extern HANDLE stageClearBuffer[2];
extern Position player;
extern MapData mapData;

void initGame();
void initEffectScreen();
void loadMainMenu();
void showLoadingStage(int);
void loadStageSelect();
void setPlayerPos(int, int);
Flag translatePlayerPos(int, int);
Flag pushBall(int, int);
Flag pushFilledBox(int, int);
void changePositionState(int, int, GameObject);
bool EqualsWithPlayerPos(int, int);
void clearScreen();
void exitGame();
void showRedEffect();
void printScreen(char*);
void releaseScreen();
void loadMapData(int);
void renderScreenToBuffer(char*);
bool checkClearStage();
void showClearStage(int);
