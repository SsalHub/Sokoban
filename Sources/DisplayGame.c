#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../Headers/BaseData.h"

Flag displayGame(int stageIndex)
{
	char buffer[_SCREEN_WIDTH_ * _SCREEN_HEIGHT_];
	char stage[32] = "";
	char input;
	Flag flag;
	
	sprintf(stage, "Stage%2d", stageIndex);
	loadMapData("Stage01");
	
	while (1)
	{
		buffer[0] = '\0';
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
			case _SPACE_:
				break;
			case _ESCAPE_:
				exitGame();
			default:
				break;
			}
		}
		
		if (flag == _STAGE_CLEAR_)
			break;
			
		renderScreenToBuffer(buffer);
		printScreen(buffer);
	}
	
	switch(flag)
	{
		case _STAGE_CLEAR_:
			return _STAGE_CLEAR_;
	}
}

