#include "../Headers/MainMenu.h"

#include <stdbool.h>
#include <conio.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenRender.h"
#include "../Headers/PlayStage.h"
#include "../Headers/UtilData.h"
#include "../Headers/ExceptionHandler.h"

void loadMainMenu()
{
	int idx = runMainMenu();
	switch(idx)
	{
		case 0:
			return;
		case 1:
			exitGame();
			return;
		default:
			throwFatalException(_UNKNOWN_EXCEPTION_);
			return;
	}
}

int runMainMenu()
{
	const int MAX_SELECT = 2;
	int selectIndex = 0;
	char input;
	
	printMainMenuScreen(selectIndex);
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _DOWN_:
					if (selectIndex + 1 < MAX_SELECT)
						selectIndex += 1;
					
					printMainMenuScreen(selectIndex);
					WaitForSeconds(0.1);
					break;
				case _UP_:
					if (0 <= selectIndex - 1)
						selectIndex -= 1;
						
					printMainMenuScreen(selectIndex);
					WaitForSeconds(0.1);
					break;
				case _SPACE_:
				case _CARRIGE_RETURN_:
					/* select ok */
					return selectIndex;
				default:
					break;
			}
		}
	}
}
