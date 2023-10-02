#include <stdbool.h>
#include <conio.h>

#include "../Headers/ScreenBuffer.h"
#include "../Headers/StageSelect.h"

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
