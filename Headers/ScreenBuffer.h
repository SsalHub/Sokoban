#pragma once

#include "BaseData.h"

typedef struct ScreenBuffer
{
	HANDLE buffer[2];
	int currentIndex;
} ScreenBuffer;

typedef struct StageSelectBox
{
	COORD pos;
	char buffer[_SCREEN_WIDTH_];
	int stageIndex;
} StageSelectBox;

//typedef enum ScreenBufferIndex
//{
//	_STAGE_SCREEN_ 				= 0,
//	_STAGE_LOADING_SCREEN_		= 1,
//	_STAGE_CLEAR_SCREEN_		= 2,
//	_STAGE_SELECT_SCREEN_		= 3,
//	_EFFECT_SCREEN_				= 4,
//	_STAGE_RESTART_SCREEN_		= 5,
//} ScreenBufferIndex;

extern ScreenBuffer screenBuffer;

void swapScreenIndex();
void printStageScreen(char*, ConsoleColor, ConsoleColor);
void printRenderedScreen(char*, ConsoleColor, ConsoleColor);
void clearScreen();
void releaseScreen();
void showStageClearScreen(int);
void showLoadingStageScreen(int);
Flag showStageRestartScreen();
int showStageSelectScreen(int, int);
void showRedEffectScreen();
void fillColorToScreen(ConsoleColor, ConsoleColor);
