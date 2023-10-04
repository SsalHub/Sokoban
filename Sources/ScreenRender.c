#include "../Headers/ScreenRender.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/ExceptionHandler.h"

ScreenBuffer screenBuffer;

void printString(char* str, COORD pos, bool bClear, bool bSwap)
{
	DWORD dw;
	char* nextLine;
	
	if (bClear) clearScreen();
	
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

void printScreen(void render(void), bool bClear, bool bSwap)
{
	if (bClear) clearScreen();
	render();
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void printMainMenuScreen(void render(int), int selectIndex, bool bClear, bool bSwap)
{
	if (bClear) clearScreen();
	render(selectIndex);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void printStageSelectScreen(void render(int, int), int maxStage, int stageIndex, bool bClear, bool bSwap)
{
	if (bClear) clearScreen();
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
	clearScreen();
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
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
}

void releaseScreen()
{
	CloseHandle(screenBuffer.buffer[0]);
	CloseHandle(screenBuffer.buffer[1]);
}

/* Red Effect */
void showRedEffect()
{
	fillColorToScreen(_RED_, _WHITE_);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
	Sleep(50);	// 0.05sec
}

/* MainMenu */
void renderMainMenuScreen(int selectIndex)
{
	ConsoleColor bColor = _DARK_PURPLE_, tColor = _WHITE_, tSelColor = _YELLOW_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) };
	COORD contentPos[2], selectedPos;
	DWORD dw;
	char title[64], content[2][64], selectedChar[5] = "▶ ";
	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	int i, contentPosY = (int)(_SCREEN_HEIGHT_ * 0.75);
	
	fillColorToScreen(bColor, tColor);
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	
	sprintf(content[0], "Game Start");
	contentPos[0].X = (int)((_SCREEN_WIDTH_ - strlen(content[0])) * 0.5);
	contentPos[0].Y = contentPosY;
	sprintf(content[1], "Exit Game");
	contentPos[1].X = (int)((_SCREEN_WIDTH_ - strlen(content[1])) * 0.5);
	contentPos[1].Y = contentPosY + 1;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
	for (i = 0; i < 2; i++)
	{
		if (i == selectIndex)
		{
			SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tSelColor | (bColor << 4));
			selectedPos.X = contentPos[i].X - 3;
			selectedPos.Y = contentPos[i].Y;
		}
		else
		{
			SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
		}
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos[i]);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content[i], strlen(content[i]), &dw, NULL);
	}
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tSelColor | (bColor << 4));
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], selectedPos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], selectedChar, strlen(selectedChar), &dw, NULL);
}
// used in post version
//void renderMainMenuScreen()
//{
//	ConsoleColor bColor = _DARK_PURPLE_, tColor = _WHITE_;
//	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.75) };
//	DWORD dw;
//	char title[64], content[64];
//	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
//	int i, j;
//	
//	/* Fill screen with background color. */
////	_beginthreadex(NULL, 0, showBlinkingString, &contentData, 0, NULL);
//	fillColorToScreen(bColor, tColor);
//	
//	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
//	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
//	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
//	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);	
//	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
//	
//	sprintf(content, "Press Any Key");
//	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
//	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos);	
//	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content, strlen(content), &dw, NULL);
//}

/* Stage Select */
void renderStageSelectScreen(int maxStage, int stageIndex)
{
	ConsoleColor bColor = _OCEAN_BLUE_, tColor = _BLACK_, tSelectedColor = _HOTPINK_, tInputColor = _YELLOW_;
	int stageSelectBoxY = (int)(_SCREEN_HEIGHT_ * 0.3), stageStructureY = (int)(_SCREEN_HEIGHT_ * 0.55);
	COORD leftArrowPos = { (int)(_SCREEN_WIDTH_ * 0.3), stageSelectBoxY }, rightArrowPos = { (int)(_SCREEN_WIDTH_ * 0.7), stageSelectBoxY };
	COORD stageStructurePos = { 0, stageStructureY }, zero = { 0, 0 };
	DWORD dw;
	StageSelectBox stageSelectBox[3];
	char leftArrowStr[3] = "◀", rightArrowStr[3] = "▶", block[3] = "□"; 
	char stageStructureString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	MapData stageStructure;
	float boxPositionWeight[3] = { -0.1, 0, 0.1 };
	int i, j, boxWidth, centerAlignX, centerAlignY;
	
	fillColorToScreen(bColor, tColor);
	
	/* Print each arrows. */
	printString(leftArrowStr, leftArrowPos, false, false);
	printString(rightArrowStr, rightArrowPos, false, false);
	
	boxWidth = strlen("┌───┐");
	/* Initialize stage select box. */
	for (i = 0; i < 3; i++)
	{
		stageSelectBox[i].stageIndex = i + (-1 + stageIndex);
		if (stageSelectBox[i].stageIndex < 1 || maxStage < stageSelectBox[i].stageIndex)
			continue;
		
		stageSelectBox[i].buffer[0] = '\0';
		sprintf(stageSelectBox[i].buffer,"%s┌────┐ \n", stageSelectBox[i].buffer);
		sprintf(stageSelectBox[i].buffer,"%s│ %03d│ \n", stageSelectBox[i].buffer, stageSelectBox[i].stageIndex);
		sprintf(stageSelectBox[i].buffer,"%s└────┘ \n", stageSelectBox[i].buffer);
		stageSelectBox[i].pos.X = (int)((_SCREEN_WIDTH_ - boxWidth) * (0.5 + boxPositionWeight[i]));
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
			
		printString(stageSelectBox[i].buffer, stageSelectBox[i].pos, false, false);
	}
	setMapData(&stageStructure, stageIndex);
	
	/* Print Stage Structure. */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	stageStructureString[0] = '\0';
	for (i = 0; i < stageStructure.width + 2; i++)
	{
		strcat(stageStructureString, block);
	}
	strcat(stageStructureString, "\n");
	for (i = 0; i < stageStructure.height; i++)
	{
		strcat(stageStructureString, block);
		for (j = 0; j < stageStructure.width; j++)
		{
			switch (stageStructure.structure[i][j])
			{
				case _NONE_:
					strcat(stageStructureString, "  ");
					break;
				case _BLOCK_:
					strcat(stageStructureString, block);
					break;
				case _BALL_:
					strcat(stageStructureString, "◎");
					break;
				case _EMPTY_BOX_:
					strcat(stageStructureString, "♤");
					break;
				case _FILLED_BOX_:
					strcat(stageStructureString, "♠");
					break;
				default:
					break;
			}
		}
		strcat(stageStructureString, block);
		strcat(stageStructureString, "\n");
	}
	for (i = 0; i < stageStructure.width + 2; i++)
	{
		strcat(stageStructureString, block);
	}
	strcat(stageStructureString, "\n");
	
	centerAlignX = (int)((_SCREEN_WIDTH_- ((stageStructure.width + 2) * 2)) * 0.5);
	stageStructurePos.X = centerAlignX;
	
	printString(stageStructureString, stageStructurePos, false, false);
}

/* Stage Loading */
void renderStageLoadingScreen()
{
	ConsoleColor bColor = _SKYBLUE_, tColor = _BLACK_;
	COORD pos = { 0, (int)(_SCREEN_HEIGHT_ * 0.5)};
	DWORD dw;
	char loadingText[_SCREEN_WIDTH_+1];
	int loop, slen;
	const int LOOPMAX = 3;
	
	fillColorToScreen(bColor, tColor);
	
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

/* Confirm Stage Restart */
void renderConfirmRestartScreen()
{
	ConsoleColor bColor = _GREEN_, tColor = _BLACK_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.7) };
	DWORD dw;
	char title[64] = "", content[64] = "";
	int i, j;
	
	fillColorToScreen(bColor, tColor);
	
	sprintf(title, 		"이 스테이지를 처음부터 다시 진행하시겠습니까?"			);
	sprintf(content, 	"← 이어서 계속 진행     |    처음부터 다시 진행 →"	);
	
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content, strlen(content), &dw, NULL);
}

/* Stage Clear */
void renderStageClearScreen()
{
	ConsoleColor bColor = _LIGHT_YELLOW_, tColor = _BLACK_;
	COORD pos = { 0, (int)(_SCREEN_HEIGHT_ * 0.5)};
	DWORD dw;
	char stageClearText[_SCREEN_WIDTH_+1];
	int loop, slen;
	const int LOOPMAX = 3;
	
	fillColorToScreen(bColor, tColor);
	
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
