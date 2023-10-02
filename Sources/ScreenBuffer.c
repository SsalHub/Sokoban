#include "../Headers/ScreenBuffer.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#include "../Headers/BaseData.h"
#include "../Headers/ExceptionHandler.h"

ScreenBuffer screenBuffer;

void printString(char* str, COORD pos, bool bSwap)
{
	DWORD dw;
	char* nextLine;
	
	clearScreen();
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void printScreen(void render(void), bool bSwap)
{
	clearScreen();
	render();
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void printStageSelectScreen(void render(int, int), int maxStage, int stageIndex, bool bSwap)
{
	clearScreen();
	render(maxStage, stageIndex);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void clearScreen()	
{
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	int i, j;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], _WHITE_ | (_BLACK_ << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}

void swapScreenIndex()
{
	screenBuffer.currentIndex = !(screenBuffer.currentIndex);
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor, bool bSwap)
{	
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1] = "";
	int i, j;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
	if (bSwap) swapScreenIndex();
}

void releaseScreen()
{
	CloseHandle(screenBuffer.buffer[0]);
	CloseHandle(screenBuffer.buffer[1]);
}

/* Red Effect */
void showRedEffect()
{
	fillColorToScreen(_RED_, _BLACK_, true);
	Sleep(50);	// 0.05sec
}

/* MainMenu */
void renderMainMenuScreen()
{
	ConsoleColor bColor = _DARK_PURPLE_, tColor = _WHITE_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.75) };
	DWORD dw;
	char title[64], content[64];
	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	int i, j;
	
	/* Fill screen with background color. */
//	_beginthreadex(NULL, 0, showBlinkingString, &contentData, 0, NULL);
	fillColorToScreen(bColor, tColor, false);
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
	
	sprintf(content, "Press Any Key");
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content, strlen(content), &dw, NULL);
}

/* Stage Select */
void renderStageSelectScreen(int maxStage, int stageIndex)
{
	ConsoleColor bColor = _OCEAN_BLUE_, tColor = _BLACK_, tSelectedColor = _RED_, tInputColor = _YELLOW_;
	int stageSelectBoxY = (int)(_SCREEN_HEIGHT_ * 0.3), stageStructureY = (int)(_SCREEN_HEIGHT_ * 0.55);
	COORD leftArrowPos = { (int)(_SCREEN_WIDTH_ * 0.3), stageSelectBoxY }, rightArrowPos = { (int)(_SCREEN_WIDTH_ * 0.7), stageSelectBoxY };
	COORD stageStructurePos = { 0, stageStructureY }, zero = { 0, 0 };
	DWORD dw;
	StageSelectBox stageSelectBox[3];
	char leftArrowStr[3] = "←", rightArrowStr[3] = "→", block[3] = "□"; 
	char stageStructure[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	char* nextLine;
	int i, j, centerAlignX, centerAlignY, currentStageIndex = stageIndex;
	
	fillColorToScreen(bColor, tColor, false);
	
	/* Print each arrows. */
	printString(leftArrowStr, leftArrowPos, false);
	printString(rightArrowStr, rightArrowPos, false);
	
	j = (int)(strlen("┌───┐\n") * 1.5);
	/* Initialize stage select box. */
	for (i = 0; i < 3; i++)
	{
		stageSelectBox[i].stageIndex = i + (-1 + currentStageIndex);
		if (stageSelectBox[i].stageIndex < 1 || maxStage < stageSelectBox[i].stageIndex)
			continue;
		
		stageSelectBox[i].buffer[0] = '\0';
		sprintf(stageSelectBox[i].buffer,"%s┌───┐\n", stageSelectBox[i].buffer);
		sprintf(stageSelectBox[i].buffer,"%s│%03d│\n", stageSelectBox[i].buffer, stageSelectBox[i].stageIndex);
		sprintf(stageSelectBox[i].buffer,"%s└───┘\n", stageSelectBox[i].buffer);
		stageSelectBox[i].pos.X = (int)(_SCREEN_WIDTH_ * 0.5) + ((-1 + i) * j);
		stageSelectBox[i].pos.Y = stageSelectBoxY;
	}
		
	/* Print Stage Select Boxes. */
	for (i = 0; i < 3; i++)
	{
		if (stageSelectBox[i].stageIndex < 1 || maxStage < stageSelectBox[i].stageIndex)
			continue;
			
		if (i == 1)
			SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tSelectedColor | (bColor << 4));
		else
			SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
			
		printString(stageSelectBox[i].buffer, stageSelectBox[i].pos, false);
	}
		
	/* Load currently selected stage's data file. */
	loadMapData(currentStageIndex);
	/* Print selected stage's structure. */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	/* Outside(border) of Map */
	stageStructure[0] = '\0';
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageStructure, block);
	}
	strcat(stageStructure, "\n");
	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(stageStructure, block);
		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			
			/* GameObjects */
			switch (mapData.map[i][j])
			{
				case _NONE_:
					strcat(stageStructure, "  ");
					break;
				case _BLOCK_:
					strcat(stageStructure, block);
					break;
				case _BALL_:
					strcat(stageStructure, "◎");
					break;
				case _EMPTY_BOX_:
					strcat(stageStructure, "♤");
					break;
				case _FILLED_BOX_:
					strcat(stageStructure, "♠");
					break;
				default:
					break;
			}
		}
		/* Outside(border) of Map */
		strcat(stageStructure, block);
		strcat(stageStructure, "\n");
	}
	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageStructure, block);
	}
	strcat(stageStructure, "\n");
	
	/* Write stage map string to screen buffer. */
	centerAlignX = (int)(((_SCREEN_WIDTH_) - ((mapData.width + 2) * 2)) * 0.5);
	stageStructurePos.X = centerAlignX;
	nextLine = strtok(stageStructure, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], stageStructurePos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		stageStructurePos.Y++;
	}
}

/* StageLoading */
void renderStageLoadingScreen()
{
	ConsoleColor bColor = _SKYBLUE_, tColor = _BLACK_;
	COORD pos = { 0, (int)(_SCREEN_HEIGHT_ * 0.5)};
	DWORD dw;
	char loadingText[_SCREEN_WIDTH_+1];
	int loop, slen;
	const int LOOPMAX = 3;
	
	fillColorToScreen(bColor, tColor, false);
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(loadingText, "Stage Loading");
	for (loop = 0; loop < LOOPMAX; loop++)
	{
		strcat(loadingText, " .");
		slen = strlen(loadingText);
		
		pos.X = (int)((_SCREEN_WIDTH_ - slen) * 0.5);
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], loadingText, slen, &dw, NULL);
		Sleep(200);	// 0.2sec
	}
}

/* ConfirmRestart */
void renderConfirmRestartScreen()
{
	ConsoleColor bColor = _GREEN_, tColor = _BLACK_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.7) };
	DWORD dw;
	char title[64] = "", content[64] = "";
	int i, j;
	
	fillColorToScreen(bColor, tColor, false);
	
	sprintf(title, 		"이 스테이지를 처음부터 다시 진행하시겠습니까?"			);
	sprintf(content, 	"← 이어서 계속 진행     |    처음부터 다시 진행 →"	);
	
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content, strlen(content), &dw, NULL);
}

/* StageClear */
void renderStageClearScreen()
{
	ConsoleColor bColor = _LIGHT_YELLOW_, tColor = _BLACK_;
	COORD pos = { 0, (int)(_SCREEN_HEIGHT_ * 0.5)};
	DWORD dw;
	char stageClearText[_SCREEN_WIDTH_+1];
	int loop, slen;
	const int LOOPMAX = 3;
	
	fillColorToScreen(bColor, tColor, false);
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(stageClearText, "Stage Clear");
	for (loop = 0; loop < LOOPMAX; loop++)
	{
		strcat(stageClearText, " .");
		slen = strlen(stageClearText);
		
		pos.X = (int)((_SCREEN_WIDTH_ - slen) * 0.5);
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], stageClearText, slen, &dw, NULL);
		Sleep(300);	// 0.3sec
	}
}

/* Stage Map Data */
void renderStageMapScreen()
{
	ConsoleColor bColor = _DARK_PURPLE_, tColor = _BLUE_, tPlayerColor = _YELLOW_;
	int centerAlignX = (int)(((_SCREEN_WIDTH_)-((mapData.width+2)*2))*0.5), centerAlignY = (int)((_SCREEN_HEIGHT_ - mapData.height)*0.5)-1;
	COORD centeredMapPos = { centerAlignX, centerAlignY }, playerPos = { centeredMapPos.X+((player.x+1)*2), centeredMapPos.Y+player.y+1 };
	COORD zero = { 0, 0 };
	DWORD dw;
	char stageMapString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char block[3] = "□", playerCharacter[5] = "★";
	char* nextLine;
	int i, j;

	clearScreen();
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	/* Outside(border) of Map */
	stageMapString[0] = '\0';
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, block);
	}
	strcat(stageMapString, "\n");
	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(stageMapString, block);
		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			/* Player */
			if (EqualsWithPlayerPos(j, i))
			{
				strcat(stageMapString, "☆");
				continue;
			}
			/* GameObjects */
			switch (mapData.map[i][j])
			{
				case _NONE_:
					strcat(stageMapString, "  ");
					break;
				case _BLOCK_:
					strcat(stageMapString, block);
					break;
				case _BALL_:
					strcat(stageMapString, "◎");
					break;
				case _EMPTY_BOX_:
					strcat(stageMapString, "♤");
					break;
				case _FILLED_BOX_:
					strcat(stageMapString, "♠");
					break;
				default:
					break;
			}
		}
		/* Outside(border) of Map */
		strcat(stageMapString, block);
		strcat(stageMapString, "\n");
	}
	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, block);
	}
	strcat(stageMapString, "\n");
	
	/* Write stage map string to screen buffer. */
	nextLine = strtok(stageMapString, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], centeredMapPos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		centeredMapPos.Y++;
	}
	centeredMapPos.X = centerAlignX;
	centeredMapPos.Y = centerAlignY;
	
	/* Write only player's character with yellow color to HANDLE(= screen buffer). */
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], playerPos);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tPlayerColor | (bColor << 4));
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
}
