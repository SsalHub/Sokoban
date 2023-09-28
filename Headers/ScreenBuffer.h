#pragma once

#include "BaseData.h"


typedef struct ScreenBuffer
{
	HANDLE buffer[2];
	int currentIndex;
} ScreenBuffer;

typedef enum ScreenBufferIndex
{
	_STAGE_SCREEN_ 				= 0,
	_STAGE_LOADING_SCREEN_		= 1,
	_STAGE_CLEAR_SCREEN_		= 2,
	_STAGE_SELECT_SCREEN_		= 3,
	_EFFECT_SCREEN_				= 4,
	_STAGE_RESTART_SCREEN_		= 5,
} ScreenBufferIndex;

extern int screenBufferLength;
extern ScreenBuffer screenBuffer[6];

ScreenBuffer* getScreenBuffer(ScreenBufferIndex);
void initEffectScreen();
void initStageRestartScreen();
void swapScreenIndex(ScreenBuffer*);
void printRenderedScreen(ScreenBuffer*, char*);
void clearScreen(ScreenBuffer*);
void showRedEffect(ScreenBuffer*);
void printScreen(ScreenBuffer*, char*);
//void printPlayer(ScreenBuffer*);
void releaseScreen();
void showStageClearScreen(int);
void showLoadingStageScreen(int);
Flag showStageRestartScreen();
