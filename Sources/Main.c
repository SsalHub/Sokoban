#include "../Headers/BaseData.h"
#include "../Headers/ScreenRender.h"
#include "../Headers/MainMenu.h"
#include "../Headers/PlayStage.h"

int main()
{
    initScreen();
	initGame();
	
	while (1)
	{
		loadMainMenu();
		playStage();
	}
	
	return 0;
}
