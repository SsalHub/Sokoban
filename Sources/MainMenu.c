#include "../Headers/MainMenu.h"

#include <stdbool.h>
#include <conio.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenRender.h"
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
