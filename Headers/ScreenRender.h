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
void initScreen();
void printString(char*, COORD, bool, bool);
void printScreen(void (void), bool, bool);
void printMainMenuScreen(void (int), int, bool, bool);
void printStageSelectScreen(void (MapData*, int), MapData*, int, bool, bool);
void clearScreen();
void swapScreenIndex();
void fillColorToScreen(ConsoleColor, ConsoleColor);
void setColor(ConsoleColor, ConsoleColor);
void releaseScreen();
void showRedEffect(); 	// Red Effect

/* Render functions */
void renderMainMenuScreen(int);				        // MainMenu
void renderStageSelectScreen(MapData*, int);        // Stage Select
void renderStageLoadingScreen();			        // Stage Loading
void renderConfirmRestartScreen();			        // Confirm Stage Restart
void renderStageClearScreen();				        // Stage Clear
void renderStageMapScreen();			     	    // Stage Map Data
