#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../Headers/BaseData.h"

void displayGame()
{
	char buffer[_SCREEN_WIDTH_ * _SCREEN_HEIGHT_];
	char input;
	
	loadMapData("Stage01");
	
	while (1)
	{
		buffer[0] = '\0';

		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
			case _LEFT_:
				if (!translatePlayerPos(-1, 0))
					showRedEffect();
				break;
			case _RIGHT_:
				if (!translatePlayerPos(1, 0))
					showRedEffect();
				break;
			case _UP_:
				if (!translatePlayerPos(0, -1))
					showRedEffect();
				break;
			case _DOWN_:
				if (!translatePlayerPos(0, 1))
					showRedEffect();
				break;
			case _SPACE_:
				break;
			case _ESCAPE_:
				exitGame();
			default:
				break;
			}
		}

		renderScreenToBuffer(buffer);
		printScreen(buffer);
	}
}

