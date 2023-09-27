#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

HANDLE screenBuffer[2];
int currentScreenBufferIndex;
HANDLE effectBuffer;
HANDLE loadingStageBuffer;
char _MAP_PATH_[16] = "../MAPS/";
Position player;
MapData mapData;

void initGame()
{
	ConsoleColor black = _BLACK_, red = _BRIGHTRED_, skyblue = _SKYBLUE_, white = _WHITE_;
	COORD pos = { 0, 0 };
	DWORD dw;
	int i, j;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	char screenInitCommand[50] = "";
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	SetConsoleTitle("Sokoban : 19 Song JaeUk in Hansung Univ.");
	
	screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	effectBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	loadingStageBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	currentScreenBufferIndex = 0;

	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	
	SetConsoleCursorInfo(screenBuffer[0], &cursor);
	SetConsoleCursorInfo(screenBuffer[1], &cursor);
	
	SetConsoleCursorInfo(effectBuffer, &cursor);
	SetConsoleTextAttribute(effectBuffer, white | (red << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	SetConsoleCursorPosition(effectBuffer, pos);
	WriteFile(effectBuffer, bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(effectBuffer, white | (black << 4));
	
	SetConsoleCursorInfo(loadingStageBuffer, &cursor);
	SetConsoleTextAttribute(effectBuffer, white | (skyblue << 4));
	WriteFile(effectBuffer, bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(effectBuffer, white | (black << 4));
}

void loadMainMenu()
{
	loadStageSelect();
}

void showLoadingStage()
{
	
}

void loadStageSelect()
{
	showLoadingStage();
	Sleep(2000);	// 2.0sec
	displayGame();
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
		case _FILLED_HOUSE_:
			if (!pushBomb(newX, newY)) return;
			
		case _NONE_:
		case _EMPTY_HOUSE_:
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
		case _FILLED_HOUSE_:
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
			mapData.Map[bombY][bombX] = _NONE_;
			mapData.Map[destY][destX] = _BOMB_;
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
	SetConsoleActiveScreenBuffer(effectBuffer);
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

void loadMapData(char* stageName)
{
	FILE *fp;
	char fname[16] = "";
	char mapDataPath[1000] = "";
	char buffer[_MAP_WIDTH_+1] = "";
	char* token;
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
		
	/* Line 1 in .skb file : Width and Height of Map */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	mapData.width = atoi(strtok(buffer, " "));
	mapData.height = atoi(strtok(buffer, " "));
	
	/* Line 2 in .skb file : Position of Player. X and Y */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	x = atoi(strtok(buffer, " "));
	y = atoi(strtok(buffer, " "));
	setPlayerPos(x, y);
	
	for (i = 0; i < mapData.height; i++)
	{
		fgets(buffer, _MAP_WIDTH_+1, fp);
		
		for (j = 0; j < mapData.width; j++)
		{
			mapData.Map[i][j] = buffer[j] - 48;
			/* Keep _EMPTY_HOUSE_ data. empty house's position data is necessary even Map data keeps changing. */
			
			// something
			
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
			if (mapData.Map[i][j] == _EMPTY_HOUSE_)		
				return false;
		}
	}
	
	return true;
}
