#include "../Headers/PlayStage.h"

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenRender.h"

void playStage()
{
	Flag flag;
	int stageIndex = 1;
	
	while(1)
	{
		stageIndex = loadStageSelect(stageIndex);
		
		loadMapData(stageIndex);
		printScreen(renderStageLoadingScreen, true);
		
		flag = playGame(stageIndex);
		
		switch (flag)
		{
			case _STAGE_CLEAR_:
				printScreen(renderStageClearScreen, true);
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
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char input;
	int maxStage, currentStage;
	
	maxStage = countMaxStage();
	currentStage = stageIndex;
	
	while (1)
	{
	
		printStageSelectScreen(renderStageSelectScreen, maxStage, currentStage, true);
		
		input = _getch();
		switch (input)
		{
			case _ESCAPE_:
			case _LEFT_:
				if (currentStage - 1 < 1) 
					showRedEffect();
				else
					currentStage = currentStage - 1;
					
				Sleep(500);	// 0.5sec
				break;
			case _RIGHT_:
				if (maxStage < currentStage + 1)
					showRedEffect();
				else
					currentStage = currentStage + 1;
					
				Sleep(500);	// 0.5sec
				break;
			case _SPACE_:
			case _CARRIGE_RETURN_:
				/* select ok */
				return currentStage;
			default:
				break;
		}
	}
}

Flag playGame(int stageIndex)
{
	char input;
	Flag flag;
	
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
					// Pause ���� ���� 
				default:
					break;
			}
		}
		
		if (flag == _STAGE_RESTART_)
			break;
		
		/* Render and Print Stage(= map) data on bufferString(= char* bufferString). */
		printScreen(renderStageMapScreen, true);
		
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
	
	printScreen(renderConfirmRestartScreen, true);
	
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

