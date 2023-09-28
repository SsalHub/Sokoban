#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

int currentScreenBufferIndex;
HANDLE stageScreenBuffer[2];
HANDLE effectBuffer;
HANDLE loadingStageBuffer[2];
HANDLE stageClearBuffer[2];
Position player;
MapData mapData;

void initGame()
{
	char screenInitCommand[50] = "";
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
	
	currentScreenBufferIndex = 0;
	stageScreenBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	stageScreenBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	effectBuffer 				= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	loadingStageBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	loadingStageBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	stageClearBuffer[0] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	stageClearBuffer[1] 		= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
//    SetCurrentConsoleFontEx(stageScreenBuffer[0], FALSE, &font);
//    SetCurrentConsoleFontEx(stageScreenBuffer[1], FALSE, &font);
	SetConsoleCursorInfo(stageScreenBuffer[0], &cursor);
	SetConsoleCursorInfo(stageScreenBuffer[1], &cursor);
	SetConsoleCursorInfo(effectBuffer, &cursor);
	SetConsoleCursorInfo(loadingStageBuffer[0], &cursor);
	SetConsoleCursorInfo(loadingStageBuffer[1], &cursor);
	SetConsoleCursorInfo(stageClearBuffer[0], &cursor);
	SetConsoleCursorInfo(stageClearBuffer[1], &cursor);

	initEffectScreen();
}

void initEffectScreen()
{
	ConsoleColor black = _BLACK_, red = _BRIGHTRED_, white = _WHITE_;
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	int i, j;
	
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
}

void loadMainMenu()
{
	loadStageSelect();
}

void showLoadingStage(int stageIndex)
{
	ConsoleColor black = _BLACK_, skyblue = _SKYBLUE_, white = _WHITE_;
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	char loadingText[_SCREEN_WIDTH_+1];
	int i, j, loop, bufferIndex = 0;
	
	SetConsoleTextAttribute(loadingStageBuffer[0], black | (skyblue << 4));
	SetConsoleTextAttribute(loadingStageBuffer[1], black | (skyblue << 4));
	sprintf(loadingText, "Stage%02d Loading", stageIndex);
	for (loop = 0; loop < 4; loop++)
	{
		strcat(loadingText, " .");
		bufferString[0] = '\0';
		for (i = 0; i < _SCREEN_HEIGHT_ ; i++)
		{
			if (i == _SCREEN_HEIGHT_ / 2)
			{
				for (j = 0; j < (_SCREEN_WIDTH_ - strlen(loadingText)) / 2; j++)
				{
					strcat(bufferString, " ");
				}
				strcat(bufferString, loadingText);
				for (j = 0; j < (_SCREEN_WIDTH_ - strlen(loadingText)) / 2; j++)
				{
					strcat(bufferString, " ");
				}
			}
			else
			{
				for (j = 0; j < _SCREEN_WIDTH_; j++)
				{
					strcat(bufferString, " ");
				}
			}
			strcat(bufferString, "\n");
		}
		SetConsoleCursorPosition(loadingStageBuffer[bufferIndex], pos);
		WriteFile(loadingStageBuffer[bufferIndex], bufferString, strlen(bufferString), &dw, NULL);
		Sleep(300);	// 0.3sec
		
		/* Change actual screen */
		SetConsoleActiveScreenBuffer(loadingStageBuffer[bufferIndex]);
		bufferIndex = !bufferIndex;
	}
	SetConsoleTextAttribute(loadingStageBuffer[0], white | (black << 4));
	SetConsoleTextAttribute(loadingStageBuffer[1], white | (black << 4));
}

void loadStageSelect()
{
	Flag flag;
	int stageIndex;
	
	/* Some Stage selecting action */
	
	//something
	
	stageIndex = 1;
	
	while(1)
	{
		showLoadingStage(stageIndex);
		
		//Sleep(2000);	// 2.0sec
		
		loadMapData(stageIndex);
		flag = displayGame(stageIndex);
		
		if (flag == _STAGE_CLEAR_)
			showClearStage(stageIndex++);
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
		case _FILLED_BOX_:
			showRedEffect();
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
		case _FILLED_BOX_:
		case _BALL_:
			showRedEffect();
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

void clearScreen()		// Clean current screen buffer.
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(stageScreenBuffer[currentScreenBufferIndex], ' ', _SCREEN_WIDTH_ * _SCREEN_HEIGHT_, pos, &dw);
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
	SetConsoleActiveScreenBuffer(stageScreenBuffer[currentScreenBufferIndex]);
}

void printScreen(char* s)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	SetConsoleCursorPosition(stageScreenBuffer[currentScreenBufferIndex], pos);
	WriteFile(stageScreenBuffer[currentScreenBufferIndex], s, strlen(s), &dw, NULL);
	SetConsoleActiveScreenBuffer(stageScreenBuffer[currentScreenBufferIndex]);
	currentScreenBufferIndex = !currentScreenBufferIndex;
}

void releaseScreen()
{
	CloseHandle(stageScreenBuffer[0]);
	CloseHandle(stageScreenBuffer[1]);
	CloseHandle(effectBuffer);
	CloseHandle(loadingStageBuffer[0]);
	CloseHandle(loadingStageBuffer[1]);
	CloseHandle(stageClearBuffer[0]);
	CloseHandle(stageClearBuffer[1]);
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
	{
		printf("Fatal Error : Failed to load map data!\n");
		printf("file name : %s, path : %s\nEnd sokoban game progresses.\n", fname, mapDataPath);
		exit(1);
	}
		
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

void renderScreenToBuffer(char* buffer)
{
	int i, j;
	char align[_SCREEN_WIDTH_] = "";
	char block[3] = "¡à";
	
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
		strcat(buffer, block);
	}
	strcat(buffer, "\n");

	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(buffer, align);
		strcat(buffer, block);

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
					strcat(buffer, block);
					break;
					
				case _BALL_:
					strcat(buffer, "¡Ý");
					break;
					
				case _EMPTY_BOX_:
					strcat(buffer, "¢»");
					break;
					
				case _FILLED_BOX_:
					strcat(buffer, "¢¼");
					break;
					
				default:
					break;
			}
		}

		/* Outside(border) of Map */
		strcat(buffer, block);

		strcat(buffer, "\n");
	}

	/* Outside(border) of Map */
	strcat(buffer, align);
	for (i = 0; i < mapData.width+2; i++)
	{
		strcat(buffer, block);
	}
	strcat(buffer, "\n");
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

void showClearStage(int stageIndex)
{
	ConsoleColor black = _BLACK_, yellow = _BRIGHTYELLOW_, white = _WHITE_;
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	char stageClearText[_SCREEN_WIDTH_+1];
	int i, j, loop, bufferIndex = 0;
	
	SetConsoleTextAttribute(stageClearBuffer[0], black | (yellow << 4));
	SetConsoleTextAttribute(stageClearBuffer[1], black | (yellow << 4));
	sprintf(stageClearText, "Stage%02d Clear", stageIndex);
	for (loop = 0; loop < 4; loop++)
	{
		strcat(stageClearText, " !");
		bufferString[0] = '\0';
		for (i = 0; i < _SCREEN_HEIGHT_ ; i++)
		{
			if (i == _SCREEN_HEIGHT_ / 2)
			{
				for (j = 0; j < (_SCREEN_WIDTH_ - strlen(stageClearText)) / 2; j++)
				{
					strcat(bufferString, " ");
				}
				strcat(bufferString, stageClearText);
				for (j = 0; j < (_SCREEN_WIDTH_ - strlen(stageClearText)) / 2; j++)
				{
					strcat(bufferString, " ");
				}
			}
			else
			{
				for (j = 0; j < _SCREEN_WIDTH_; j++)
				{
					strcat(bufferString, " ");
				}
			}
			strcat(bufferString, "\n");
		}
		SetConsoleCursorPosition(stageClearBuffer[bufferIndex], pos);
		WriteFile(stageClearBuffer[bufferIndex], bufferString, strlen(bufferString), &dw, NULL);
		Sleep(300);	// 0.3sec
		/* Change actual screen */
		SetConsoleActiveScreenBuffer(stageClearBuffer[bufferIndex]);
		bufferIndex = !bufferIndex;
	}
	SetConsoleTextAttribute(stageClearBuffer[0], white | (black << 4));
	SetConsoleTextAttribute(stageClearBuffer[1], white | (black << 4));
}
