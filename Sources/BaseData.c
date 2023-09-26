#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

HANDLE screenBuffer[3];
int currentScreenBufferIndex;
char _MAP_PATH_[16] = "../MAPS/";
Position player;
MapData mapData;

void initGame()
{
	ConsoleColor black = _BLACK_, red = _BRIGHTRED_, white = _WHITE_;
	COORD pos = { 0, 0 };
	DWORD dw;
	int i, j;
	char effectBuffer[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	char screenInitCommand[50] = "";
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	SetConsoleTitle("Sokoban : Hansung Univ.");
	
	screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer[_EFFECT_SCREEN_] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	currentScreenBufferIndex = 0;

	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(screenBuffer[0], &cursor);
	SetConsoleCursorInfo(screenBuffer[1], &cursor);
	SetConsoleCursorInfo(screenBuffer[_EFFECT_SCREEN_], &cursor);
	SetConsoleTextAttribute(screenBuffer[_EFFECT_SCREEN_], white | (red << 4));
	effectBuffer[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(effectBuffer, " ");
		}
		strcat(effectBuffer, "\n");
	}
	SetConsoleCursorPosition(screenBuffer[_EFFECT_SCREEN_], pos);
	WriteFile(screenBuffer[_EFFECT_SCREEN_], effectBuffer, strlen(effectBuffer), &dw, NULL);
	SetConsoleTextAttribute(screenBuffer[_EFFECT_SCREEN_], white | (black << 4));
}

void setPlayerPos(int x, int y)
{
	player.x = x;
	player.y = y;
}

/* Translate player's position. If success it returns true, or false. */
bool translatePlayerPos(int x, int y)
{
	int newX = player.x + x, newY = player.y + y;
	
	/* If player tried get out of the Map */
	if (newX < 0 || _MAP_WIDTH_ <= newX)
	{
		showRedEffect();
		return false;
	}
	if (newY < 0 || _MAP_HEIGHT_ <= newY)
	{
		showRedEffect();
		return false;
	}
	
	/* Check the position where player moved on. */
	switch (mapData.Map[newY][newX])
	{
		case _BLOCK_:
			showRedEffect();
			return false;
			
		case _BOMB_:
			if (!pushBomb(newX, newY)) return;
			
		case _NONE_:
		case _EMPTY_HOUSE_:
		case _FILLED_HOUSE_:
			player.x = newX;
			player.y = newY;
			break;
	}
	
	return true;
}

bool pushBomb(int bombX, int bombY)
{
	int destX = bombX + (bombX - player.x), destY = bombY + (bombY - player.y);
	
	switch (mapData.Map[destY][destX])
	{
		case _BLOCK_:
			return false;
			
		case _EMPTY_HOUSE_:
			mapData.Map[bombY][bombX] = _NONE_;
			mapData.Map[destY][destX] = _FILLED_HOUSE_;
			if (checkClearStage())
			{
				/* Stage Clear Action */
				
				// something
				
			}
		
		case _NONE_:
		case _FILLED_HOUSE_:
			break;
	}
	
	return true;
}

bool EqualsWithPlayerPos(int x, int y)
{
	if (player.x == x && player.y == y)
		return true;
	return false;
}

void clearScreen()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(screenBuffer[currentScreenBufferIndex], ' ', _SCREEN_WIDTH_ * _SCREEN_HEIGHT_, pos, &dw);
}

void exitGame()
{
	system("cls");
	printf("THANKS FOR PLAYING\n\n\n\n\n\n\n");
	exit(0);
}

void showRedEffect()
{
	SetConsoleActiveScreenBuffer(screenBuffer[_EFFECT_SCREEN_]);
	Sleep(10); // 0.05sec
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
}

void printScreen(char* s)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	SetConsoleCursorPosition(screenBuffer[currentScreenBufferIndex], pos);
	WriteFile(screenBuffer[currentScreenBufferIndex], s, strlen(s), &dw, NULL);
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
	currentScreenBufferIndex = !currentScreenBufferIndex;
}

void releaseScreen()
{
	CloseHandle(screenBuffer[0]);
	CloseHandle(screenBuffer[1]);
	CloseHandle(screenBuffer[2]);
}

void setConsoleColor(int background, int text)
{
	ConsoleColor c = _RED_;
	SetConsoleTextAttribute(screenBuffer[_EFFECT_SCREEN_], text | (background << 4));
	SetConsoleActiveScreenBuffer(screenBuffer[_EFFECT_SCREEN_]);
}

void loadMapData(char* stageName)
{
	FILE *fp;
	char fname[16] = "";
	char mapDataPath[1000] = "";
	char buffer[_MAP_WIDTH_+1] = "";
	int i, j, x, y;
	
	_getcwd(mapDataPath, 1000);		// path of root of this project directory
	sprintf(fname, "%s.skb", stageName);
	sprintf(mapDataPath, "%s\\Maps\\%s", mapDataPath, fname);
	
	fp = fopen(mapDataPath, "r");
	if (fp == NULL)
	{
		printf("Fatal Error : Failed to load map data!\n");
		printf("file name : %s, path : %s\nEnd sokoban game progresses.\n", fname, mapDataPath);
		exit(1);
	}
		
	/* Line 1~2 in .skb file : Width and Height of Map */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	mapData.width = atoi(buffer);
	fgets(buffer, _MAP_WIDTH_+1, fp);
	mapData.height = atoi(buffer);
	
	/* Line 3~4 in .skb file : Position of Player. X and Y */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	x = atoi(buffer);
	fgets(buffer, _MAP_WIDTH_+1, fp);
	y = atoi(buffer);
	setPlayerPos(x, y);
	
	for (i = 0; i < mapData.height; i++)
	{
		fgets(buffer, _MAP_WIDTH_+1, fp);
		
		for (j = 0; j < mapData.width; j++)
		{
			mapData.Map[i][j] = buffer[j] - 48;
		}
	}	
}

void renderScreenToBuffer(char* buffer)
{
	int i, j;
	char align[_SCREEN_WIDTH_] = "";
	
	clearScreen();
	
	/* Align center */
	for (i = 0; i < ((_SCREEN_WIDTH_)-((mapData.width+2)*2)) / 2; i++)
	{
		strcat(align, " ");
	}
	for (i = 0; i < (_SCREEN_HEIGHT_ - mapData.height) / 2 - 1; i++)
	{
		strcat(buffer, "\n");
	}

	/* Outside(border) of Map */
	strcat(buffer, align);
	for (i = 0; i < mapData.width+2; i++)
	{
		strcat(buffer, "!!");
	}
	strcat(buffer, "\n");

	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(buffer, align);
		strcat(buffer, "!!");

		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			/* Player */
			if (EqualsWithPlayerPos(j, i))
			{
				strcat(buffer, "¡Ù");
				continue;
			}

			/* GameObjects */
			switch (mapData.Map[i][j])
			{
				case _NONE_:
					strcat(buffer, "  ");
					break;
					
				case _BLOCK_:
					strcat(buffer, "[]");
					break;
					
				case _BOMB_:
					strcat(buffer, "£À");
					break;
					
				case _EMPTY_HOUSE_:
					strcat(buffer, "¢»");
					break;
					
				case _FILLED_HOUSE_:
					strcat(buffer, "¢¼");
					break;
					
				default:
					break;
			}
		}

		/* Outside(border) of Map */
		strcat(buffer, "!!");

		strcat(buffer, "\n");
	}

	/* Outside(border) of Map */
	strcat(buffer, align);
	for (i = 0; i < mapData.width+2; i++)
	{
		strcat(buffer, "!!");
	}
	strcat(buffer, "\n");
}

bool checkClearStage()
{
	int i, j;
	
	for (i = 0; i < mapData.height; i++)
	{
		for (j = 0; j < mapData.width; j++)
		{
			if (mapData.Map[i][j] == _EMPTY_HOUSE_)		// 
				return false;
		}
	}
	
	return true;
}
