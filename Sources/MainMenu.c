#include <stdbool.h>
#include <conio.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/PlayStage.h"

void loadMainMenu()
{
	int stageIndex = 1;
	
	renderMainMenuScreen();
	printScreen(renderMainMenuScreen, true);
	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}
	
	playStage();
}
