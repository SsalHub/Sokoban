#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "../Headers/BaseData.h"

Flag displayGame(int stageIndex)
{
	char buffer[_SCREEN_WIDTH_ * _SCREEN_HEIGHT_];
	char input;
	Flag flag;
	
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
			
		renderScreenToBuffer(buffer);
		printScreen(buffer);
		
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

