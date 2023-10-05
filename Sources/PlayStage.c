#include "../Headers/PlayStage.h"

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/UtilData.h"
#include "../Headers/ScreenRender.h"

void playStage()
{
	Flag flag;
	int stageIndex = 1;
	
	flag = _STAGE_SELECT_;
	while(1)
	{
		if (flag == _STAGE_SELECT_)
			stageIndex = loadStageSelect(stageIndex);
		
		loadMapData(&mapData, stageIndex);
		printScreen(renderStageLoadingScreen, true, true);
		
		flag = playGame(stageIndex);
		
		switch (flag)
		{
			case _STAGE_CLEAR_:
				printScreen(renderStageClearScreen, true, true);
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
	char input;
	int maxStage, currStage;
	
	maxStage = countMaxStage();
	currStage = stageIndex;
	
	printStageSelectScreen(renderStageSelectScreen, findMapData(currStage), maxStage, true, true);
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _ESCAPE_:
				case _LEFT_:
					if (1 < currStage)
						currStage -= 1;
					
					printStageSelectScreen(renderStageSelectScreen, findMapData(currStage), maxStage, true, true);
					WaitForSeconds(1);
					break;
				case _RIGHT_:
					if (currStage < maxStage)
						currStage += 1;
						
					printStageSelectScreen(renderStageSelectScreen, findMapData(currStage), maxStage, true, true);
					WaitForSeconds(1);
					break;
				case _SPACE_:
				case _CARRIGE_RETURN_:
					/* select ok */
					return currStage;
				default:
					break;
			}
		}
	}
}

Flag playGame(int stageIndex)
{
	Flag flag;
	char input;
	
//	cleanInputBuffer();
	
	while (1)
	{
		flag = _FALSE_;

		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _LEFT_:
					flag = translatePlayerPos(-1, 0);
					break;
				case _RIGHT_:
					flag = translatePlayerPos(1, 0);
					break;
				case _UP_:
					flag = translatePlayerPos(0, -1);
					break;
				case _DOWN_:
					flag = translatePlayerPos(0, 1);
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
		printScreen(renderStageMapScreen, true, true);
		
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
	
	printScreen(renderConfirmRestartScreen, true, true);
	
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

