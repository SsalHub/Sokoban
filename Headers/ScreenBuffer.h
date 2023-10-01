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

extern ScreenBuffer screenBuffer;

void swapScreenIndex();
void renderStageMapScreen();
void renderToCurrentScreen(char*, COORD, ConsoleColor, ConsoleColor);
void renderRedEffectScreen();
void printScreen();
void printMainMenuScreen();
void printRenderedScreen(char*, ConsoleColor, ConsoleColor);
void clearScreen();
void releaseScreen();
void showStageClearScreen(int);
void showLoadingStageScreen(int);
Flag showStageRestartScreen();
int showStageSelectScreen(int, int);
void showMainMenuScreen();
void showRedEffectScreen();
void fillColorToScreen(ConsoleColor, ConsoleColor);
