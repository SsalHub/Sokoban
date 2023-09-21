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
	_NONE_,
	_BLOCK_,
	_HOUSE_,
	_BOMB_,
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

#define EFFECT_SCREEN 2
extern HANDLE screenBuffer[3];
extern int currentScreenBufferIndex;
extern int Map[_MAP_HEIGHT_][_MAP_WIDTH_];
extern Position player;

void initGame();
void initMap();
void setPlayerPos(int, int);
bool translatePlayerPos(int, int);
bool EqualsWithPlayerPos(int, int);
void clearScreen();
void exitGame();
void showRedEffect();
void printScreen(char*);
void releaseScreen();
void setConsoleColor(int, int);
