#pragma once
#include <stdbool.h>
#include <Windows.h>

#define _SCREEN_WIDTH_ 120
#define _SCREEN_HEIGHT_ 30
#define _MAP_WIDTH_ 20
#define _MAP_HEIGHT_ 20

typedef enum ConsoleColor
{
	_BLACK_,
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

typedef enum GameObject
{
	_NONE_ = 0,
	_BLOCK_ = 1,
	_BOMB_ = 2,
	_EMPTY_HOUSE_ = 3,
	_FILLED_HOUSE_ = 4,
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
	int Map[_MAP_HEIGHT_][_MAP_WIDTH_];
} MapData;

extern HANDLE screenBuffer[2];
extern int currentScreenBufferIndex;
extern HANDLE effectBuffer;
extern HANDLE loadingStageBuffer;
extern Position player;
extern MapData mapData;

void initGame();
void loadMainMenu();
void showLoadingStage();
void loadStageSelect();
void setPlayerPos(int, int);
bool translatePlayerPos(int, int);
bool pushBomb(int, int);
bool EqualsWithPlayerPos(int, int);
void clearScreen();
void exitGame();
void showRedEffect();
void printScreen(char*);
void releaseScreen();
void loadMapData();
void renderScreenToBuffer(char*);
bool checkClearStage();
