#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <direct.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/ExceptionHandler.h"

Position player;
MapData mapData;

void fortestfunc()
{
	printf("hi"); exit(0);
}

void initGame()
{
	char screenInitCommand[50] = "";
	int i;
	
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	SetConsoleTitle("Sokoban : 19 Song JaeUk in Hansung Univ.");
	
	/* Cursor */
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	
	/* Font */
//	CONSOLE_FONT_INFOEX font = {sizeof(font)};
//    font.dwFontSize.X = 15;
//    font.dwFontSize.Y = 30;
	
	for (i = 0; i < screenBufferLength; i++)
	{
		screenBuffer[i].currentIndex = 0;
		screenBuffer[i].buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(screenBuffer[i].buffer[0], &cursor);
		screenBuffer[i].buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(screenBuffer[i].buffer[1], &cursor);
	}
	
//	currentScreenBufferIndex = 0;
//	stageScreenBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageScreenBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	loadingStageBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	loadingStageBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageClearBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageClearBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageSelectBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageSelectBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	effectBuffer 				= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	stageRestartBuffer 			= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
//    SetCurrentConsoleFontEx(stageScreenBuffer[0], FALSE, &font);
//    SetCurrentConsoleFontEx(stageScreenBuffer[1], FALSE, &font);
//	SetConsoleCursorInfo(stageScreenBuffer[0], &cursor);
//	SetConsoleCursorInfo(stageScreenBuffer[1], &cursor);
//	SetConsoleCursorInfo(loadingStageBuffer[0], &cursor);
//	SetConsoleCursorInfo(loadingStageBuffer[1], &cursor);
//	SetConsoleCursorInfo(stageClearBuffer[0], &cursor);
//	SetConsoleCursorInfo(stageClearBuffer[1], &cursor);
//	SetConsoleCursorInfo(stageSelectBuffer[0], &cursor);
//	SetConsoleCursorInfo(stageSelectBuffer[1], &cursor);
//	SetConsoleCursorInfo(effectBuffer, &cursor);
//	SetConsoleCursorInfo(stageRestartBuffer, &cursor);

	initEffectScreen();
	initStageRestartScreen();
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
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	if (newY < 0 || mapData.height <= newY)
	{
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	
	/* Check the position where player moved on. */
	switch (mapData.map[newY][newX])
	{
		case _BLOCK_:
			showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
			return _BLOCKED_;
			
		case _FILLED_BOX_:
			if (pushFilledBox(newX, newY) == _BLOCKED_) 
				return _BLOCKED_;
			else
				break;
		case _BALL_:
			switch (pushBall(newX, newY))
			{
				case _TRUE_:
					break;
				case _BLOCKED_:
					return _BLOCKED_;
				case _STAGE_CLEAR_:
					setPlayerPos(newX, newY);
					return _STAGE_CLEAR_;
			}
			
		case _NONE_:
		case _EMPTY_BOX_:
			break;
	}
	
	setPlayerPos(newX, newY);
	return _TRUE_;
}

Flag pushBall(int ballX, int ballY)
{
	int destX = ballX + (ballX - player.x), destY = ballY + (ballY - player.y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || mapData.width <= destX)
	{
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	if (destY < 0 || mapData.height <= destY)
	{
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	
	switch (mapData.map[destY][destX])
	{
		case _BLOCK_:
		case _FILLED_BOX_:
			showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
			return _BLOCKED_;
			
		case _EMPTY_BOX_:
			changePositionState(destX, destY, _FILLED_BOX_);
			if (checkClearStage()) return _STAGE_CLEAR_;
			break;
		
		case _NONE_:
			changePositionState(destX, destY, _BALL_);
			break;
	}
	
	changePositionState(ballX, ballY, _NONE_);
	return _TRUE_;
}

Flag pushFilledBox(int boxX, int boxY)
{
	int destX = boxX + (boxX - player.x), destY = boxY + (boxY - player.y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || mapData.width <= destX)
	{
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	if (destY < 0 || mapData.height <= destY)
	{
		showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
		return _BLOCKED_;
	}
	
	switch (mapData.map[destY][destX])
	{
		case _BLOCK_:
		case _FILLED_BOX_:
		case _BALL_:
			showRedEffect(getScreenBuffer(_STAGE_SCREEN_));
			return _BLOCKED_;
			
		case _EMPTY_BOX_:
			changePositionState(destX, destY, _FILLED_BOX_);
			break;
		
		case _NONE_:
			changePositionState(destX, destY, _BALL_);
			break;
	}
	
	changePositionState(boxX, boxY, _EMPTY_BOX_);
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
	if (o == _BALL_ || o == _FILLED_BOX_)
	{
		mapData.map[y][x] = o;
		return;
	}
	if (mapData.structure[y][x] == _EMPTY_BOX_)
	{
		mapData.map[y][x] = _EMPTY_BOX_;
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

void exitGame()
{
	system("cls");
	printf("THANKS FOR PLAYING\n\n\n\n\n\n\n");
	exit(0);
}

void loadMapData(int stageIndex)
{
	FILE *fp;
	char fname[16] = "";
	char mapDataPath[1000] = "";
	char buffer[_MAP_WIDTH_+1] = "";
	char* token;
	int i, j, x, y;
	
	_getcwd(mapDataPath, 1000);		// path of root of this project directory
	sprintf(fname, "Stage%02d.skb", stageIndex);
	sprintf(mapDataPath, "%s\\Maps\\%s", mapDataPath, fname);
	
	fp = fopen(mapDataPath, "r");
	if (fp == NULL)
		throwFatalException(_STAGE_FILE_NOT_FOUND_);
		
	/* Line 1 in .skb file : Width and Height of Map */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	mapData.width = atoi(strtok(buffer, " "));
	mapData.height = atoi(buffer);
	
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
				case _EMPTY_BOX_:
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

void renderStageMap(char* bufferString)
{
	int i, j;
	char align[_SCREEN_WIDTH_] = "";
	char block[3] = "¡à";
	
	/* Align center */
	for (i = 0; i < (int)(((_SCREEN_WIDTH_)-((mapData.width+2)*2)) * 0.5); i++)
	{
		strcat(align, " ");
	}
	for (i = 0; i < (int)((_SCREEN_HEIGHT_ - mapData.height) * 0.5) - 1; i++)
	{
		strcat(bufferString, "\n");
	}

	/* Outside(border) of Map */
	strcat(bufferString, align);
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(bufferString, block);
	}
	strcat(bufferString, "\n");

	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(bufferString, align);
		strcat(bufferString, block);

		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			/* Player */
			if (EqualsWithPlayerPos(j, i))
			{
				strcat(bufferString, "¡Ù");
				continue;
			}

			/* GameObjects */
			switch (mapData.map[i][j])
			{
				case _NONE_:
					strcat(bufferString, "  ");
					break;
					
				case _BLOCK_:
					strcat(bufferString, block);
					break;
					
				case _BALL_:
					strcat(bufferString, "¡Ý");
					break;
					
				case _EMPTY_BOX_:
					strcat(bufferString, "¢»");
					break;
					
				case _FILLED_BOX_:
					strcat(bufferString, "¢¼");
					break;
					
				default:
					break;
			}
		}

		/* Outside(border) of Map */
		strcat(bufferString, block);

		strcat(bufferString, "\n");
	}

	/* Outside(border) of Map */
	strcat(bufferString, align);
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(bufferString, block);
	}
	strcat(bufferString, "\n");
}

bool checkClearStage()
{
	int i;
	for (i = 0; i < mapData.houseCount; i++)
	{
		if (mapData.map[mapData.house[i].y][mapData.house[i].x] != _FILLED_BOX_)
			return false;
	}
	return true;
}
