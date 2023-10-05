#include "../Headers/PlayStage.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <process.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/UtilData.h"
#include "../Headers/ScreenRender.h"

void playStage()
{
	MapDataDLL* pMapDLL;
	Flag flag;
	int stageIndex = 1;
	
	flag = _STAGE_SELECT_;
	while(1)
	{
		if (flag == _STAGE_SELECT_)
			stageIndex = loadStageSelect(stageIndex);
		
//		_beginthreadex(NULL, 0, printStageLoadingScreen, (void*)&bPaused, 0, NULL);
		printStageLoadingScreen(stageIndex);
		pMapDLL = findMapDataDLL(stageIndex);
		
		flag = playGame(&(pMapDLL->mapData));
		
		switch (flag)
		{
			case _STAGE_CLEAR_:
				printStageClearScreen(stageIndex);
				stageIndex++;
				break;
			case _STAGE_RESTART_:
				/* Get back to loading screen without increasing stageIndex. */
				break;	
			case _STAGE_SELECT_:
				break;	
			default:
				break;
		}
	}
}

int loadStageSelect(int stageIndex)
{
	Flag flag;
	MapDataDLL* pMapDLL;
	char input;
	int maxStage;
	
	maxStage = countMaxStage();
	pMapDLL = findMapDataDLL(stageIndex);		// stageIndex = 1
	
	printStageSelectScreen(&(pMapDLL->mapData), maxStage);
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _ESCAPE_:
				case _LEFT_:
					if (1 < pMapDLL->mapData.stageIndex)
						pMapDLL = pMapDLL->before;
					printStageSelectScreen(&(pMapDLL->mapData), maxStage);
					WaitForSeconds(0.3);
					break;
				case _RIGHT_:
					if (pMapDLL->mapData.stageIndex < maxStage)
						pMapDLL = pMapDLL->after;
					printStageSelectScreen(&(pMapDLL->mapData), maxStage);
					WaitForSeconds(0.3);
					break;
				case _SPACE_:
				case _CARRIGE_RETURN_:
					/* select ok */
					return pMapDLL->mapData.stageIndex;
				default:
					break;
			}
		}
	}
}

Flag playGame(MapData* pMap)
{
	Flag flag;
	MapData map;
	char input;
	
//	cleanInputBuffer();
	
	flag = _FALSE_;
	copyMapData(&map, pMap);
	setPlayerPos(&map, map.playerBeginPos.X, map.playerBeginPos.Y);
	
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _LEFT_:
					flag = translatePlayerPos(&map, -1, 0);
					break;
				case _RIGHT_:
					flag = translatePlayerPos(&map, 1, 0);
					break;
				case _UP_:
					flag = translatePlayerPos(&map, 0, -1);
					break;
				case _DOWN_:
					flag = translatePlayerPos(&map, 0, 1);
					break;
				case _UPPER_R_:
				case _LOWER_R_:
					if (confirmRestartStage() == _STAGE_RESTART_)
						flag = _STAGE_RESTART_;
					break;
				case _SPACE_:
					break;
				case _ESCAPE_:
					// Pause 구현 예정 
				default:
					break;
			}
		}
		
		if (flag == _STAGE_RESTART_)
			break;
		
		/* Render and Print Stage(= map) data on bufferString(= char* bufferString). */
		printStageMapScreen(&map);
		
		if (flag == _STAGE_CLEAR_)
			break;
	}
	
	switch(flag)
	{
		case _STAGE_CLEAR_:
			return _STAGE_CLEAR_;
		case _STAGE_RESTART_:
			return _STAGE_RESTART_;
	}
}

Flag confirmRestartStage()
{
	char input;
	
	printScreen(renderConfirmRestartScreen);
	
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _ESCAPE_:
				case _LEFT_:
					return _FALSE_;
				case _RIGHT_:
					return _STAGE_RESTART_;
			}
		}
	}
}

