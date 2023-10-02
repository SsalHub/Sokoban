#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>	// about console
#include <direct.h>		// about reading dir and file
#include <dirent.h>		// about reading dir and file
#include <process.h>	// about thread

#include "../Headers/ScreenRender.h"
#include "../Headers/ExceptionHandler.h"

Position player;
MapData mapData;
MapDataDLL* head;
MapDataDLL* tail;

void fortestfunc()
{
	printf("hello\n"); exit(0);
}

void initGame()
{
	char screenInitCommand[50] = "";
	int i, maxStage;
	MapDataDLL* node;
	head = NULL;
	tail = NULL;
	
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	SetConsoleTitle("Sokoban : 19 Song JaeUk in Hansung Univ.");
	
	/* Cursor */
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	
	screenBuffer.currentIndex = 0;
	screenBuffer.buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[0], &cursor);
	screenBuffer.buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[1], &cursor);
	
	/* Load all map data */
	maxStage = countMaxStage();
	for (i = 1; i <= maxStage; i++)
	{
		if (i == 1)
		{
			node = (MapDataDLL*)malloc(sizeof(MapDataDLL));
			node.before = NULL;
			node.after = NULL;
			loadMapData(&(node.mapData), i);
			head = node;
			continue;
		}
		
		if (i == maxStage)
		{
			node.after = (MapDataDLL*)malloc(sizeof(MapDataDLL));
			node.after.before = node;
			node = node.after;
			node.after = NULL;
			loadMapData(&(node.mapData), i);
			tail = node;
			break;
		}
		
		node.after = (MapDataDLL*)malloc(sizeof(MapDataDLL));
		node.after.before = node;
		node = node.after;
		node.after = NULL;
		loadMapData(&(node.mapData), i);
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
	1. Ball & Filled Box
	2. Empty Box (not on 'mapData.map[][]' array, 'mapData.structure[][]')
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
	releaseMapDataDLL();
	system("cls");
	printf("THANKS FOR PLAYING\n\n\n\n\n\n\n");
	exit(0);
}

bool checkClearStage()
{
	int i;
	for (i = 0; i < mapData.boxCount; i++)
	{
		if (mapData.map[mapData.box[i].y][mapData.box[i].x] != _FILLED_BOX_)
			return false;
	}
	return true;
}

int countMaxStage()
{
	char mapDataPath[1000] = "";
    struct dirent *dir;
    DIR *dp;
    int stageCount;

	_getcwd(mapDataPath, 1000);		// path of root of this project directory
	strcat(mapDataPath, "\\Maps\\");
	stageCount = 0;
	
	/* If stage map data dir not exists */
    if ((dp = opendir(mapDataPath)) == NULL)
    {
		throwFatalException(_STAGE_FILE_NOT_FOUND_);
    }

    while ((dir = readdir(dp)) != NULL)
    {
    	/* If found file is myself or parent dir */
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        stageCount++;
    }
    
    return stageCount;
}

void loadMapData(MapData* dest, int stageIndex)
{
	FILE *fp;
	char mapDataPath[1000] = "";
	char buffer[_MAP_WIDTH_+1] = "";
	char fname[16] = "";
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
	dest->width = atoi(strtok(buffer, " "));
	dest->height = atoi(buffer);
	dest->stageIndex = stageIndex;
	
	mapData.boxCount = 0;
	for (i = 0; i < mapData.height; i++)
	{
		fgets(buffer, _MAP_WIDTH_+1, fp);
		
		for (j = 0; j < mapData.width; j++)
		{
			dest->structure[i][j] = buffer[j] - 48;
			switch (buffer[j] - 48)
			{
//				case _PLAYER_:
//					setPlayerPos(j, i);
//					break;
				case _EMPTY_BOX_:
					dest->box[mapData.boxCount].x = j;
					dest->box[mapData.boxCount].y = i;
					dest->boxCount++;
					// It continues to default below.
				default:
					dest->map[i][j] = buffer[j] - 48;
			}
		}
	}	
}

void releaseMapDataDLL()
{
	MapDataDLL* node = head;
	MapDataDLL* del;
	
	while (node != tail)
	{
		del = node;
		node = node->after;
		free(del);
	}
	free(tail);
	
	head = NULL;
	tail = NULL;
}
