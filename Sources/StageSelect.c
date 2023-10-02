#include "../Headers/StageSelect.h"

#include <Windows.h>
#include <direct.h>
#include <dirent.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/ExceptionHandler.h"

void loadStageSelect(int stageIndex)
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
