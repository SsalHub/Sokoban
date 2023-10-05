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
MapDataDLL* head;
MapDataDLL* tail;
int maxStage;

void fortestfunc(char* s)
{
	printf("%s\n", s); exit(0);
}

void initGame()
{
	int i;
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
}
/* Must add something */
void exitGame()
{
	releaseMapDataDLL();
	exit(0);
}
/* Must add something */
void YouWonThisGame()
{
	releaseMapDataDLL();
	exit(0);
	// or just show celebration screen, and return to main menu
}

/* Load and Read game files, and Initialize game datas about stage map data. */

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
	dest->currentMove = 0;
	dest->history = NULL;
	
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
				    dest->originMap[i][j] = _NONE_;
				    break;
				case 'p':
//				    dest->structure[i][j] = _PLAYER_;
				    dest->originMap[i][j] = _PLAYER_;
				    dest->playerBeginPos.X = j;
				    dest->playerBeginPos.Y = i;
				    dest->currPlayerPos = dest->playerBeginPos;
					break;
				case 'w':
				    dest->originMap[i][j] = _WALL_;
				    break;
				case 'a':
				    dest->originMap[i][j] = _BALL_;
				    break;
				case 'b':
				    dest->originMap[i][j] = _EMPTY_BOX_;
					dest->originalBoxesPos[dest->boxCount].X = j;
					dest->originalBoxesPos[dest->boxCount].Y = i;
					dest->boxCount++;
					break;
			}
		}
	}
	memmove(dest->currMap, dest->originMap, _MAP_WIDTH_*_MAP_HEIGHT_*sizeof(GameObject));
}

void pushPlayerHistory(MapData* map, COORD pos)
{
	if (map->history == NULL)
	{
		map->history = (PlayerHistory*)malloc(sizeof(PlayerHistory));
		memmove(&(map->history->movement), &pos, sizeof(COORD));
		map->history->before = NULL;
		map->history->after = NULL;
		return;
	}
	
	PlayerHistory* node = (PlayerHistory*)malloc(sizeof(PlayerHistory));
	memmove(&(node->movement), &pos, sizeof(COORD));
	map->history->after = node;
	node->before = map->history;
	node->after = NULL;
	map->history = node;
}

void releaseMapDataDLL()
{
	MapDataDLL* node;
	MapDataDLL* del;
	
	node = head;
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

void releasePlayerHistory(MapData* map)
{
	PlayerHistory* node;
	PlayerHistory* del;
	
	node = map->history;
	while (node != NULL)
	{
		del = node;
		node = node->after;
		free(del);
	}
	map->history = NULL;
}

MapDataDLL* findMapDataDLL(int stageIndex)
{
	MapDataDLL* node;
	
	node = head;
	while (node != NULL && 1 < stageIndex)
	{
	   node = node->after;
	   stageIndex--;
    }
	return node;
}

void copyMapData(MapData* dest, MapData* src)
{
	dest->stageIndex = src->stageIndex;
	dest->width = src->width;
	dest->height = src->height;
	dest->boxCount = src->boxCount;
	dest->currentMove = src->currentMove;
	memmove(dest->currMap, src->currMap, _MAP_WIDTH_*_MAP_HEIGHT_*sizeof(GameObject));
	memmove(dest->originMap, src->originMap, _MAP_WIDTH_*_MAP_HEIGHT_*sizeof(GameObject));
	memmove(&(dest->playerBeginPos), &(src->playerBeginPos), sizeof(COORD));
	memmove(&(dest->currPlayerPos), &(src->currPlayerPos), sizeof(COORD));
	memmove(&(dest->originalBoxesPos), &(src->originalBoxesPos), _MAP_WIDTH_*_MAP_HEIGHT_*sizeof(COORD));
	dest->history = src->history;
}


/* Control playing stage and player. */


void setPlayerPos(MapData* map, int x, int y)
{
	map->currPlayerPos.X = x;
	map->currPlayerPos.Y = y;
}

/* Translate player's position. If success it returns true, or false. */
Flag translatePlayerPos(MapData* map, int x, int y)
{
	COORD movement = { x, y };
	int newX = map->currPlayerPos.X + x, newY = map->currPlayerPos.Y + y;
	
	/* If player tried get out of the Map */
	if (newX < 0 || map->width <= newX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (newY < 0 || map->height <= newY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	/* Check the position where player moved on. */
	switch (map->currMap[newY][newX])
	{
		case _WALL_:
			showRedEffect();
			return _BLOCKED_;
			
		case _FILLED_BOX_:
			if (pushFilledBox(map, newX, newY) == _BLOCKED_) 
				return _BLOCKED_;
			else
				break;
		case _BALL_:
			switch (pushBall(map, newX, newY))
			{
				case _TRUE_:
					break;
				case _BLOCKED_:
					return _BLOCKED_;
				case _STAGE_CLEAR_:
					setPlayerPos(map, newX, newY);
					return _STAGE_CLEAR_;
			}
			
		case _NONE_:
		case _EMPTY_BOX_:
			break;
	}
	
	setPlayerPos(map, newX, newY);
	map->currentMove++;
	pushPlayerHistory(map, movement);
	return _TRUE_;
}

Flag pushBall(MapData* map, int ballX, int ballY)
{
	int destX = ballX + (ballX - map->currPlayerPos.X), destY = ballY + (ballY - map->currPlayerPos.Y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || map->width <= destX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (destY < 0 || map->height <= destY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	switch (map->currMap[destY][destX])
	{
		case _WALL_:
		case _FILLED_BOX_:
			showRedEffect();
			return _BLOCKED_;
			
		case _EMPTY_BOX_:
			changePositionState(map, destX, destY, _FILLED_BOX_);
			if (checkClearStage(map)) return _STAGE_CLEAR_;
			break;
		
		case _NONE_:
			changePositionState(map, destX, destY, _BALL_);
			break;
	}
	
	changePositionState(map, ballX, ballY, _NONE_);
	return _TRUE_;
}

Flag pushFilledBox(MapData* map, int boxX, int boxY)
{
	int destX = boxX + (boxX - map->currPlayerPos.X), destY = boxY + (boxY - map->currPlayerPos.Y);
	
	/* If tried to push out of the Map */
	if (destX < 0 || map->width <= destX)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	if (destY < 0 || map->height <= destY)
	{
		showRedEffect();
		return _BLOCKED_;
	}
	
	switch (map->currMap[destY][destX])
	{
		case _WALL_:
		case _FILLED_BOX_:
		case _BALL_:
			showRedEffect();
			return _BLOCKED_;
			
		case _EMPTY_BOX_:
			changePositionState(map, destX, destY, _FILLED_BOX_);
			break;
		
		case _NONE_:
			changePositionState(map, destX, destY, _BALL_);
			break;
	}
	
	changePositionState(map, boxX, boxY, _EMPTY_BOX_);
	return _TRUE_;
}

void changePositionState(MapData* map, int x, int y, GameObject o)
{
	/*
	GameObject Print Priority
	1. Ball & Filled Box
	2. Empty Box (not on 'mapData.map[][]' array, 'mapData.structure[][]')
	3. etc.. (like _NONE_)
	*/
	if (o == _BALL_ || o == _FILLED_BOX_)
	{
		map->currMap[y][x] = o;
		return;
	}
	if (map->originMap[y][x] == _EMPTY_BOX_)
	{
		map->currMap[y][x] = _EMPTY_BOX_;
		return;
	}
	map->currMap[y][x] = o;
}

bool EqualsWithPlayerPos(MapData* map, int x, int y)
{
	if (map->currPlayerPos.X == x && map->currPlayerPos.Y == y)
		return true;
	return false;
}

Flag undoPlayerAction(MapData* map)
{
	if (map->history == NULL)
		return _FALSE_;
	
	PlayerHistory* del;
	Flag flag;
	COORD pos;
	
	memmove(&pos, &(map->history->movement), sizeof(COORD));
	del = map->history;
	map->history = map->history->before;
	free(del);
	
	flag = translatePlayerPos(map, -pos.X, -pos.Y);
	if (flag == _TRUE_) map->currentMove--;
	
	return flag;
}

bool checkClearStage(MapData* map)
{
	int i;
	
	for (i = 0; i < map->boxCount; i++)
	{
		if (map->currMap[map->originalBoxesPos[i].Y][map->originalBoxesPos[i].X] != _FILLED_BOX_)
			return false;
	}
	return true;
}
