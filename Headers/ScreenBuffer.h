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

/* Basic screen functions */
void printString(char*, COORD, bool);
void printScreen(void*(void), bool);
void printStageSelectScreen(void*(int, int), bool);
void clearScreen();
void swapScreenIndex();
void fillColorToScreen(ConsoleColor, ConsoleColor, bool);
void releaseScreen();

/* Render functions */
void showRedEffect(); 				// Red Effect
void renderMainMenuScreen();		// MainMenu
void renderStageSelectScreen();		// Stage Select
void renderStageLoadingScreen();	// Stage Loading
void renderConfirmRestartScreen();	// Confirm Stage Restart
void renderStageClearScreen();		// Stage Clear
void renderStageMapScreen();		// Stage Map Data
