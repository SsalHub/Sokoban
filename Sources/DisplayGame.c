#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../Headers/BaseData.h"

void displayGame()
{
	int stage = 1;
	char buffer[_SCREEN_WIDTH_ * _SCREEN_HEIGHT_];
	char stage[32] = "";
	char input;
	
	sprintf(stage, "Stage%2d", stage);
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
				translatePlayerPos(-1, 0);
				break;
			case _RIGHT_:
				translatePlayerPos(1, 0);
				break;
			case _UP_:
				translatePlayerPos(0, -1);
				break;
			case _DOWN_:
				translatePlayerPos(0, 1);
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

