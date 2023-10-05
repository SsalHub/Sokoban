#include "../Headers/ScreenRender.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/UtilData.h"
#include "../Headers/ExceptionHandler.h"

ScreenBuffer screenBuffer;

void initScreen()
{
	CONSOLE_CURSOR_INFO cursor;
	char screenInitCommand[50] = "";
	
    sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	SetConsoleTitle("Sokoban : 19 Song JaeUk in Hansung Univ.");
	
	/* Cursor */
	cursor.dwSize = 1;
	cursor.bVisible = false;
	
	screenBuffer.currentIndex = 0;
	screenBuffer.buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[0], &cursor);
	screenBuffer.buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[1], &cursor);
}

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

void printStageSelectScreen(void render(MapData*, int), MapData* map, int maxStage, bool bClear, bool bSwap)
{
	if (bClear) clearScreen();
	render(map, maxStage);
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
	char bufferString[(_SCREEN_WIDTH_+1)*_SCREEN_HEIGHT_+1] = "";
	int i, currIdx;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
        currIdx = i * (_SCREEN_WIDTH_ + 1);
	    memset(bufferString+currIdx, ' ', _SCREEN_WIDTH_ * sizeof(char));
		bufferString[currIdx+_SCREEN_WIDTH_] = '\n';
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}

void setColor(ConsoleColor bColor, ConsoleColor tColor)
{
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
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
	WaitForSeconds(0.05);
}

/* MainMenu */
void renderMainMenuScreen(int selectIndex)
{
	ConsoleColor bColor = _HOTPINK_, tColor = _BLACK_, tSelColor = _BLUE_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) };
	COORD contentPos[2], selectedPos;
	DWORD dw;
	char title[64], content[2][64], selectedChar[5] = "▶ ";
	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	int i, contentPosY = (int)(_SCREEN_HEIGHT_ * 0.75);
	
	fillColorToScreen(bColor, tColor);
	
	setColor(bColor, tColor);
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
			selectedPos.X = contentPos[i].X - 3;
			selectedPos.Y = contentPos[i].Y;
	        setColor(bColor, tSelColor);
		}
		else
		{
	        setColor(bColor, tColor);
		}
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos[i]);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content[i], strlen(content[i]), &dw, NULL);
	}
	setColor(bColor, tSelColor);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], selectedPos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], selectedChar, strlen(selectedChar), &dw, NULL);
}

/* Stage Select */
void renderStageSelectScreen(MapData* map, int maxStage)
{
	ConsoleColor bColor = _HOTPINK_, bBlankColor = _WHITE_, tColor = _BLACK_, bStructColor = _DARK_PURPLE_, tStructColor = _GREEN_;
	COORD leftArrowPos = { _SCREEN_WIDTH_ * 0.08, _SCREEN_HEIGHT_ * 0.5 }, rightArrowPos = { (int)(_SCREEN_WIDTH_ * 0.92), _SCREEN_HEIGHT_ * 0.5 };
	COORD titlePos = { 0, _SCREEN_HEIGHT_ * 0.1 };
	COORD structurePos = { (_SCREEN_WIDTH_ - _MAP_WIDTH_) * 0.5, (_SCREEN_HEIGHT_ - _MAP_HEIGHT_) * 0.5 + 2 };
	char leftArrowStr[5] = "◀", rightArrowStr[5] = "▶", block[5] = "□";
	char title[_SCREEN_WIDTH_ * 5], structure[(_MAP_WIDTH_+1)*_MAP_HEIGHT_+1];
	DWORD dw;
	int i, j, currIdx;
	
	fillColorToScreen(bColor, tColor);
	
	/* Print Each Arrows. */
	if (1 < map->stageIndex)
	   printString(leftArrowStr, leftArrowPos, false, false);
	if (map->stageIndex < maxStage)
	   printString(rightArrowStr, rightArrowPos, false, false);
	
	/* Print Title. */
	title[0] = '\0';
	sprintf(title, "%s┌────────────────────────────┐ \n", title);
	titlePos.X = (_SCREEN_WIDTH_ - (strlen(title) * 0.5)) * 0.5;
	sprintf(title, "%s│          STAGE %03d         │ \n", title, map->stageIndex);
	sprintf(title, "%s└────────────────────────────┘ \n", title);
	printString(title, titlePos, false, false);
	
	/* Print Stage Structure. */
	setColor(bBlankColor, tColor);
	for (i = 0; i < _MAP_HEIGHT_; i++)
	{
		currIdx = i * (_MAP_WIDTH_ + 1);
		memset(structure + currIdx, ' ', _MAP_WIDTH_ * sizeof(char));
		structure[currIdx+_MAP_WIDTH_] = '\n';
	}
	structure[(_MAP_WIDTH_+1)*_MAP_HEIGHT_] = '\0';
	printString(structure, structurePos, false, false);
	
	setColor(bStructColor, tStructColor);
	structure[0] = '\0';
	for (i = 0; i < map->width+2; i++)
	{
	   strcat(structure, character[_OUT_OF_MAP_]);
    }
	strcat(structure, "\n");
	for (i = 0; i < map->height; i++)
	{
	   strcat(structure, character[_OUT_OF_MAP_]);
	   for (j = 0; j < map->width; j++)
	   {
	       switch (map->structure[i][j])
	       {
	           case _NONE_:
	               strcat(structure, character[_NONE_]);
	               break;
	           case _PLAYER_:
	               strcat(structure, character[_PLAYER_]);
	               break;
	           case _BLOCK_:
	               strcat(structure, character[_BLOCK_]);
	               break;
	           case _BALL_:
	               strcat(structure, character[_BALL_]);
	               break;
	           case _EMPTY_BOX_:
	               strcat(structure, character[_EMPTY_BOX_]);
	               break;
	           case _FILLED_BOX_:
	               strcat(structure, character[_FILLED_BOX_]);
	               break;
           }
       }
	   strcat(structure, character[_OUT_OF_MAP_]);
	   strcat(structure, "\n");
    }
	strcat(structure, "\n");
	for (i = 0; i < map->width+2; i++)
	{
	   strcat(structure, character[_OUT_OF_MAP_]);
    }
    
    structurePos.X = (_SCREEN_WIDTH_ - ((map->width + 2) * 2)) * 0.5;
    structurePos.Y = (_SCREEN_HEIGHT_ - map->height) * 0.5 + 1;
	printString(structure, structurePos, false, false);
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
	
	setColor(bColor, tColor);
	sprintf(loadingText, "Stage Loading");
	for (loop = 0; loop < LOOPMAX; loop++)
	{
		strcat(loadingText, " .");
		slen = strlen(loadingText);
		
		pos.X = (int)((_SCREEN_WIDTH_ - slen) * 0.5);
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], loadingText, slen, &dw, NULL);
		WaitForSeconds(0.2);
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
	
	setColor(bColor, tColor);
	sprintf(stageClearText, "Stage Clear");
	for (loop = 0; loop < LOOPMAX; loop++)
	{
		strcat(stageClearText, " .");
		slen = strlen(stageClearText);
		
		pos.X = (int)((_SCREEN_WIDTH_ - slen) * 0.5);
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], stageClearText, slen, &dw, NULL);
		WaitForSeconds(0.3);
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
	char* nextLine;
	int i, j;
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	/* Outside(border) of Map */
	stageMapString[0] = '\0';
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, character[_OUT_OF_MAP_]);
	}
	strcat(stageMapString, "\n");
	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(stageMapString, character[_OUT_OF_MAP_]);
		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			/* Player */
			if (EqualsWithPlayerPos(j, i))
			{
		        strcat(stageMapString, character[_PLAYER_]);
				continue;
			}
			/* GameObjects */
			switch (mapData.map[i][j])
			{
				case _NONE_:
		            strcat(stageMapString, character[_NONE_]);
					break;
				case _BLOCK_:
		            strcat(stageMapString, character[_BLOCK_]);
					break;
				case _BALL_:
		            strcat(stageMapString, character[_BALL_]);
					break;
				case _EMPTY_BOX_:
		            strcat(stageMapString, character[_EMPTY_BOX_]);
					break;
				case _FILLED_BOX_:
		            strcat(stageMapString, character[_FILLED_BOX_]);
					break;
				default:
					break;
			}
		}
		/* Outside(border) of Map */
		strcat(stageMapString, character[_OUT_OF_MAP_]);
		strcat(stageMapString, "\n");
	}
	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, character[_OUT_OF_MAP_]);
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
	setColor(bColor, tPlayerColor);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], character[_PLAYER_], strlen(character[_PLAYER_]), &dw, NULL);
}
