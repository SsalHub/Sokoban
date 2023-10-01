#pragma once

#include <stdbool.h>

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

extern ScreenBuffer screenBuffer;

void printScreen(char*, COORD, bool);
void printScreen(void*(void), bool);
void clearScreen();
void swapScreenIndex();
void fillColorToScreen(ConsoleColor, ConsoleColor, bool);
void releaseScreen();
void showRedEffect();

/* Utils 				- must be removed */
void renderToCurrentScreen(char*, COORD, ConsoleColor, ConsoleColor);
void printRenderedScreen(char*, ConsoleColor, ConsoleColor);

/* Stage etc... 		- must be removed */
void renderStageMapScreen();
void showStageClearScreen(int);
void showLoadingStageScreen(int);
Flag showStageRestartScreen();
int showStageSelectScreen(int, int);

/* Red Effect 			- must be removed */
void renderRedEffectScreen();
void showRedEffectScreen();
