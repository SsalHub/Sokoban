#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/PlayStage.h"

void playStage()
{
	int stageIndex = 1;
	
	while(1)
	{
		stageIndex = loadStageSelect(stageIndex);
		
		loadMapData(stageIndex);
		printScreen(renderStageLoadingScreen, true);
		
		flag = displayGame(stageIndex);
		
		switch (flag)
		{
			case _STAGE_CLEAR_:
				showStageClearScreen(stageIndex++);
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

void playGame(int stageIndex)
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
					if (showStageRestartScreen() == _STAGE_RESTART_)
						flag = _STAGE_RESTART_;
					break;
				case _SPACE_:
					break;
				case _ESCAPE_:
					exitGame();
				default:
					break;
			}
		}
		
		if (flag == _STAGE_RESTART_)
			break;
		
		/* Render and Print Stage(= map) data on bufferString(= char* bufferString). */
		renderStageMapScreen();
		printScreen();
		swapScreenIndex();
		
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

