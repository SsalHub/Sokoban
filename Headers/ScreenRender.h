#pragma once

#include <stdbool.h>
#include <process.h>

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
void printScreen(void (void));
void printMainMenuScreen(int);
void printStageSelectScreen(MapData*, int);
void printStageLoadingScreen(int);
//unsigned _stdcall printStageLoadingScreen(void*);
void printStageMapScreen(MapData*);
void printStageClearScreen();
void clearScreen();
void swapScreenIndex();
void fillColorToScreen(ConsoleColor, ConsoleColor);
void setColor(ConsoleColor, ConsoleColor);
void releaseScreen();
void showRedEffect(); 	// Red Effect

/* Render functions */
void renderString(char*, COORD);
void renderMainMenuScreen(int);					// MainMenu
void renderStageSelectScreen(MapData*, int);	// Stage Select
void renderStageLoadingScreen(int, int);		// Stage Loading
void renderConfirmRestartScreen();				// Confirm Stage Restart
void renderStageClearScreen(int, int);			// Stage Clear
void renderStageMapScreen(MapData*);			// Stage Map Data
void renderPlayer(COORD, ConsoleColor, ConsoleColor);	// Player only, with player personal color
