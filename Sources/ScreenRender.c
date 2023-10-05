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

void printScreen(void render(void))
{
	clearScreen();
	render();
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void printMainMenuScreen(int selectIndex)
{
	clearScreen();
	renderMainMenuScreen(selectIndex);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void printStageSelectScreen(MapData* map, int maxStage)
{
	clearScreen();
	renderStageSelectScreen(map, maxStage);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void printStageLoadingScreen(int stageIndex)
{
	const int LOOPMAX = 3;
	int i;
	
	for (i = 0; i < LOOPMAX; i++)
	{
		clearScreen();
		renderStageLoadingScreen(stageIndex, i);
		SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
		swapScreenIndex();
		WaitForSeconds(0.2);
	}
}
//unsigned _stdcall printStageLoadingScreen(void* pPaused)
//{
//	const int LOOPMAX = 3;
//	bool* bPaused = (bool*)pPaused;
//	int i;
//	
//	for (i = 0; i < LOOPMAX; i++)
//	{
//		clearScreen();
//		renderStageLoadingScreen(i);
//		SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
//		swapScreenIndex();
//		WaitForSeconds(0.2);
//	}
//	*bPaused = false;
//}

void printStageMapScreen(MapData* pMap)
{
	clearScreen();
	renderStageMapScreen(pMap);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void printStageClearScreen(int stageIndex)
{
	const int LOOPMAX = 5;
	int i = 0;
	
	for (i = 0; i < LOOPMAX; i++)
	{
		clearScreen();
		renderStageClearScreen(stageIndex, i);
		SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
		swapScreenIndex();
		WaitForSeconds(0.3);
	}
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
	    memset(bufferString+currIdx, ' ', _SCREEN_WIDTH_*sizeof(char));
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

void renderString(char* str, COORD pos)
{
	DWORD dw;
	char* nextLine;
	
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
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
	int i, contentPosY = _SCREEN_HEIGHT_ * 0.75;
	
	fillColorToScreen(bColor, tColor);
	
	setColor(bColor, tColor);
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (_SCREEN_WIDTH_ - strlen(title)) * 0.5;
	
	sprintf(content[0], "Game Start");
	contentPos[0].X = (_SCREEN_WIDTH_ - strlen(content[0])) * 0.5;
	contentPos[0].Y = contentPosY;
	sprintf(content[1], "Exit Game");
	contentPos[1].X = (_SCREEN_WIDTH_ - strlen(content[1])) * 0.5;
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
	ConsoleColor bColor = _HOTPINK_, bBlankColor = _DARK_PURPLE_, tColor = _BLACK_, bStructColor = _DARK_PURPLE_, tStructColor = _GREEN_;
	COORD leftArrowPos = { _SCREEN_WIDTH_ * 0.08, _SCREEN_HEIGHT_ * 0.5 }, rightArrowPos = { _SCREEN_WIDTH_ * 0.92, _SCREEN_HEIGHT_ * 0.5 };
	COORD titlePos = { 0, _SCREEN_HEIGHT_ * 0.1 };
	COORD structurePos = { (_SCREEN_WIDTH_ - (_MAP_WIDTH_ * 1.5)) * 0.5, (_SCREEN_HEIGHT_ - _MAP_HEIGHT_) * 0.5 + 2 };
	COORD playerPos;
	char leftArrowStr[5] = "◀", rightArrowStr[5] = "▶";
	char title[_SCREEN_WIDTH_ * 5], structure[(int)(_MAP_WIDTH_*1.5+1)*_MAP_HEIGHT_+1];
	DWORD dw;
	int i, j, currIdx;
	
	fillColorToScreen(bColor, tColor);
	
	/* Print Each Arrows. */
	if (1 < map->stageIndex)
	   renderString(leftArrowStr, leftArrowPos);
	if (map->stageIndex < maxStage)
	   renderString(rightArrowStr, rightArrowPos);
	
	/* Print Title. */
	title[0] = '\0';
	sprintf(title, "%s┌────────────────────────────┐ \n", title);
	titlePos.X = (_SCREEN_WIDTH_ - (strlen(title) * 0.5)) * 0.5;
	sprintf(title, "%s│          STAGE %03d         │ \n", title, map->stageIndex);
	sprintf(title, "%s└────────────────────────────┘ \n", title);
	renderString(title, titlePos);
	
	/* Print Stage Structure. */
	setColor(bBlankColor, tColor);
	for (i = 0; i < _MAP_HEIGHT_; i++)
	{
		currIdx = i * (int)(_MAP_WIDTH_ * 1.5 + 1);
		memset(structure + currIdx, ' ', (int)(_MAP_WIDTH_ * 1.5) * sizeof(char));
		structure[currIdx+(int)(_MAP_WIDTH_*1.5)] = '\n';
	}
	structure[(int)(_MAP_WIDTH_*1.5+1)*_MAP_HEIGHT_] = '\0';
	renderString(structure, structurePos);
	
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
	       switch (map->map[_ORIGIN_MAP_INDEX_][i][j])
	       {
	           case _NONE_:
	               strcat(structure, character[_NONE_]);
	               break;
	           case _PLAYER_:
	               strcat(structure, character[_PLAYER_]);
	               break;
	           case _WALL_:
	               strcat(structure, character[_WALL_]);
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
	renderString(structure, structurePos);
	playerPos.X = structurePos.X + (map->playerBeginPos.X + 1) * 2;
	playerPos.Y = structurePos.Y + (map->playerBeginPos.Y + 1);
	renderPlayer(playerPos, bStructColor, _YELLOW_);
}

/* Stage Loading */
void renderStageLoadingScreen(int stageIndex, int loop)
{
	ConsoleColor bColor = _SKYBLUE_, tColor = _BLACK_;
	COORD pos = { 0, _SCREEN_HEIGHT_ * 0.5};
	DWORD dw;
	char loadingText[_SCREEN_WIDTH_+1];
	int i, slen;
	
	fillColorToScreen(bColor, tColor);
	
	setColor(bColor, tColor);
	sprintf(loadingText, "Stage%03d Loading", stageIndex);
	for (i = 0; i < loop; i++)
		strcat(loadingText, " .");
		
	slen = strlen(loadingText);
	
	pos.X = (_SCREEN_WIDTH_ - slen) * 0.5;
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], loadingText, slen, &dw, NULL);
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
void renderStageClearScreen(int stageIndex, int loop)
{
	ConsoleColor bColor = _LIGHT_YELLOW_, tColor = _BLACK_;
	COORD pos = { 0, (int)(_SCREEN_HEIGHT_ * 0.5)};
	DWORD dw;
	char stageClearText[_SCREEN_WIDTH_+1];
	int i, slen;
	
	fillColorToScreen(bColor, tColor);
	
	setColor(bColor, tColor);
	sprintf(stageClearText, "Stage%03d Clear", stageIndex);
	for (i = 0; i < loop; i++)
		strcat(stageClearText, " .");
		
	slen = strlen(stageClearText);
	
	pos.X = (_SCREEN_WIDTH_ - slen) * 0.5;
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], stageClearText, slen, &dw, NULL);
}

/* Stage Map Data */
void renderStageMapScreen(MapData* map)
{
	ConsoleColor bColor = _HOTPINK_, bBlankColor = _DARK_PURPLE_, tColor = _BLUE_, tPlayerColor = _YELLOW_;
	int centerAlignX = ((_SCREEN_WIDTH_)-((map->width+2)*2))*0.5, centerAlignY = (_SCREEN_HEIGHT_ - map->height)*0.5-1;
	COORD centeredMapPos = { centerAlignX, centerAlignY }, playerPos = { centeredMapPos.X+((map->currPlayerPos.X+1)*2), centeredMapPos.Y+map->currPlayerPos.Y+1 };
	DWORD dw;
	char stageMapString[(_SCREEN_WIDTH_*2+1)*_SCREEN_HEIGHT_+1];
	int i, j;
	
	fillColorToScreen(bBlankColor, tColor);
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	stageMapString[0] = '\0';
	for (i = 0; i < map->width+2; i++)
	{
		strcat(stageMapString, character[_OUT_OF_MAP_]);
	}
	strcat(stageMapString, "\n");
	for (i = 0; i < map->height; i++)
	{
		strcat(stageMapString, character[_OUT_OF_MAP_]);
		for (j = 0; j < map->width; j++)
		{
//			if (EqualsWithPlayerPos(j, i))
//			{
//		        strcat(stageMapString, character[_PLAYER_]);
//				continue;
//			}
			switch (map->map[_CURRENT_MAP_INDEX_][i][j])
			{
				case _NONE_:
		            strcat(stageMapString, character[_NONE_]);
					break;
				case _WALL_:
		            strcat(stageMapString, character[_WALL_]);
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
			}
		}
		strcat(stageMapString, character[_OUT_OF_MAP_]);
		strcat(stageMapString, "\n");
	}
	for (i = 0; i < map->width + 2; i++)
	{
		strcat(stageMapString, character[_OUT_OF_MAP_]);
	}
	strcat(stageMapString, "\n");
	
	/* Write stage map string to screen buffer. */
	setColor(bColor, tColor);
	renderString(stageMapString, centeredMapPos);
	renderPlayer(playerPos, bColor, tPlayerColor);
}

void renderPlayer(COORD playerPos, ConsoleColor bColor, ConsoleColor tColor)
{
	DWORD dw;
	
	setColor(bColor, tColor);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], playerPos);
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], character[_PLAYER_], strlen(character[_PLAYER_]), &dw, NULL);
}
