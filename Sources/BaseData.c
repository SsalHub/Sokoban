#include "../Headers/BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>	// about console
#include <direct.h>		// about reading dir and file
#include <dirent.h>		// about reading dir and file
#include <process.h>	// about thread

#include "../Headers/ScreenRender.h"
#include "../Headers/UtilData.h"
#include "../Headers/ExceptionHandler.h"

char character[7][5];
Position player;
MapData mapData;
MapDataDLL* head;
MapDataDLL* tail;

void fortestfunc(char* s)
{
	printf("%s\n", s); exit(0);
}

void initGame()
{
	int i, maxStage;
	MapDataDLL* node;
	head = NULL;
	tail = NULL;
	
	sprintf(character[_NONE_],         "  ");
	sprintf(character[_PLAYER_],       "¡Ú");
	sprintf(character[_WALL_],         "¡á");
	sprintf(character[_BALL_],         "¡Ý");
	sprintf(character[_EMPTY_BOX_],    "¢»");
	sprintf(character[_FILLED_BOX_],   "¢¼");
	sprintf(character[_OUT_OF_MAP_],   "¡Ø");
	
	/* Load all map data */
	maxStage = countMaxStage();
	for (i = 1; i <= maxStage; i++)
	{
		if (i == 1)
		{
			node = (MapDataDLL*)malloc(sizeof(MapDataDLL));
			node->before = NULL;
			node->after = NULL;
			loadMapData(&(node->mapData), i);
			head = node;
			continue;
		}
		
		if (i == maxStage)
		{
			node->after = (MapDataDLL*)malloc(sizeof(MapDataDLL));
			node->after->before = node;
			node = node->after;
			node->after = NULL;
			loadMapData(&(node->mapData), i);
			tail = node;
			break;
		}
		
		node->after = (MapDataDLL*)malloc(sizeof(MapDataDLL));
		node->after->before = node;
		node = node->after;
		node->after = NULL;
		loadMapData(&(node->mapData), i);
	}
	
	setPlayerPos(0, 0);
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
		case _WALL_:
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
		case _WALL_:
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
		case _WALL_:
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
		if (mapData.map[mapData.originalBoxesPos[i].y][mapData.originalBoxesPos[i].x] != _FILLED_BOX_)
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
	
	dest->stageIndex = stageIndex;
	
	/* Line 1 in .skb file : Width and Height of Map */
	fgets(buffer, _MAP_WIDTH_+1, fp);
	dest->width = atoi(strtok(buffer, " "));
	dest->height = atoi(strtok(NULL, " "));
	
	dest->boxCount = 0;
	for (i = 0; i < dest->height; i++)
	{
		fgets(buffer, _MAP_WIDTH_+1, fp);
		
		for (j = 0; j < dest->width; j++)
		{
			switch (buffer[j])
			{
				case '-':
				    dest->structure[i][j] = _NONE_;
				    dest->map[i][j] = _NONE_;
				    break;
				case 'p':
				    dest->map[i][j] = _PLAYER_;
				    dest->playerBeginPos.x = j;
				    dest->playerBeginPos.y = i;
					break;
				case 'w':
				    dest->structure[i][j] = _WALL_;
				    dest->map[i][j] = _WALL_;
				    break;
				case 'a':
				    dest->structure[i][j] = _BALL_;
				    dest->map[i][j] = _BALL_;
				    break;
				case 'b':
				    dest->structure[i][j] = _EMPTY_BOX_;
				    dest->map[i][j] = _EMPTY_BOX_;
					dest->boxCount++;
					dest->originalBoxesPos[mapData.boxCount].x = j;
					dest->originalBoxesPos[mapData.boxCount].y = i;
					break;
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

MapData* findMapData(int index)
{
	MapDataDLL* node;
	int stageIndex = index;
	
	node = head;
	while (node != NULL && 1 < stageIndex)
	{
	   node = node->after;
	   stageIndex--;
    }
	return &(node->mapData);
}
