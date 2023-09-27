#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

HANDLE screenBuffer[2];
int currentScreenBufferIndex;
HANDLE effectBuffer;
HANDLE loadingStageBuffer;
HANDLE stageClearBuffer;
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
	stageClearBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	currentScreenBufferIndex = 0;

	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	
	SetConsoleCursorInfo(screenBuffer[0], &cursor);
	SetConsoleCursorInfo(screenBuffer[1], &cursor);
	
	SetConsoleCursorInfo(effectBuffer, &cursor);
	bufferString[0] = '\0';
	SetConsoleTextAttribute(effectBuffer, white | (red << 4));
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
	bufferString[0] = '\0';
	SetConsoleTextAttribute(loadingStageBuffer, white | (skyblue << 4));
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	SetConsoleCursorPosition(loadingStageBuffer, pos);
	WriteFile(loadingStageBuffer, bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(loadingStageBuffer, white | (black << 4));
	
	SetConsoleCursorInfo(stageClearBuffer, &cursor);
	bufferString[0] = '\0';
	SetConsoleTextAttribute(stageClearBuffer, white | (red << 4));
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	SetConsoleCursorPosition(stageClearBuffer, pos);
	WriteFile(stageClearBuffer, bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(stageClearBuffer, white | (black << 4));
}

void loadMainMenu()
{
	loadStageSelect();
}

void showLoadingStage()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	SetConsoleActiveScreenBuffer(loadingStageBuffer);
	//Sleep(2000);	// 2.0sec
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
}

void loadStageSelect()
{
	Flag flag;
	
	while(1)
	{
		showLoadingStage();
		
		//Sleep(2000);	// 2.0sec
		
		flag = displayGame(1);
		if (flag == _STAGE_CLEAR_)
			showClearStage();
		else
			break;
	}
}

void setPlayerPos(int x, int y)
{
	player.x = x;
	player.y = y;
}

/* Translate player's position. If success it returns true, or false. */
Flag translatePlayerPos(int x, int y)
{
	int newX = player.x + x, newY = player.y + y;
	
	/* If player tried get out of the Map */
	if (newX < 0 || mapData.width <= newX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (newY < 0 || mapData.height <= newY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	/* Check the position where player moved on. */
	switch (mapData.map[newY][newX])
	{
		case _BLOCK_:
			showRedEffect();
			return _BLOCKED_;
			
		case _FILLED_HOUSE_:
			if (pushFilledHouse(newX, newY) == _BLOCKED_) 
				return _BLOCKED_;
			else
				break;
		case _BOMB_:
			switch (pushBomb(newX, newY))
			{
				case _TRUE_:
					break;
				case _BLOCKED_:
					return _BLOCKED_;
				case _STAGE_CLEAR_:
					player.x = newX;
					player.y = newY;
					return _STAGE_CLEAR_;
			}
			
		case _NONE_:
		case _EMPTY_HOUSE_:
			break;
	}
	
	player.x = newX;
	player.y = newY;
	return _TRUE_;
}

Flag pushBomb(int bombX, int bombY)
{
	int destX = bombX + (bombX - player.x), destY = bombY + (bombY - player.y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || mapData.width <= destX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (destY < 0 || mapData.height <= destY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	switch (mapData.map[destY][destX])
	{
		case _BLOCK_:
		case _FILLED_HOUSE_:
			showRedEffect();
			return _BLOCKED_;
			
		case _EMPTY_HOUSE_:
			changePositionState(destX, destY, _FILLED_HOUSE_);
			if (checkClearStage()) return _STAGE_CLEAR_;
			break;
		
		case _NONE_:
			changePositionState(destX, destY, _BOMB_);
			break;
	}
	
	changePositionState(bombX, bombY, _NONE_);
	return _TRUE_;
}

Flag pushFilledHouse(int houseX, int houseY)
{
	int destX = houseX + (houseX - player.x), destY = houseY + (houseY - player.y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || mapData.width <= destX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (destY < 0 || mapData.height <= destY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	switch (mapData.map[destY][destX])
	{
		case _BLOCK_:
		case _FILLED_HOUSE_:
			showRedEffect();
			return _BLOCKED_;
			
		case _EMPTY_HOUSE_:
			changePositionState(destX, destY, _FILLED_HOUSE_);
			break;
		
		case _NONE_:
			changePositionState(destX, destY, _BOMB_);
			break;
	}
	
	changePositionState(houseX, houseY, _EMPTY_HOUSE_);
	return _TRUE_;
}


void changePositionState(int x, int y, GameObject o)
{
	/*
	GameObject Print Priority
	1. Bomb & Filled House
	2. Empty House (not on 'mapData.map[][]' array, 'mapData.structure[][]')
	3. etc.. (like _NONE_)
	*/
	if (o == _BOMB_ || o == _FILLED_HOUSE_)
	{
		mapData.map[y][x] = o;
		return;
	}
	if (mapData.structure[y][x] == _EMPTY_HOUSE_)
	{
		mapData.map[y][x] = _EMPTY_HOUSE_;
		return;
	}
	mapData.map[y][x] = o;
}

bool EqualsWithPlayerPos(int x, int y)
{
	if (player.x == x && player.y == y)
		return true;
	return false;
}

void clearScreen()		// Clean current screen buffer.
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
	
	mapData.houseCount = 0;
	for (i = 0; i < mapData.height; i++)
	{
		fgets(buffer, _MAP_WIDTH_+1, fp);
		
		for (j = 0; j < mapData.width; j++)
		{
			mapData.structure[i][j] = buffer[j] - 48;
			switch (buffer[j] - 48)
			{
				case _PLAYER_:
					setPlayerPos(j, i);
					break;
				case _EMPTY_HOUSE_:
					mapData.house[mapData.houseCount].x = j;
					mapData.house[mapData.houseCount].y = i;
					mapData.houseCount++;
					// It continues to default below.
				default:
					mapData.map[i][j] = buffer[j] - 48;
			}
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
			switch (mapData.map[i][j])
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
	int i;
	for (i = 0; i < mapData.houseCount; i++)
	{
		if (mapData.map[mapData.house[i].y][mapData.house[i].x] != _FILLED_HOUSE_)
			return false;
	}
	return true;
}

void showClearStage()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	SetConsoleActiveScreenBuffer(stageClearBuffer);
	Sleep(2000);	// 2.0sec
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
}
