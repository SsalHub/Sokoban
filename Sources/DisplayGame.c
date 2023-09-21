#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "BaseData.h"

void renderScreenToBuffer(char* buffer)
{
	clearScreen();

	/* Outside(border) of Map */
	for (int i = 0; i < _MAP_WIDTH_+2; i++)
	{
		strcat(buffer, "!!");
	}
	strcat(buffer, "\n");

	for (int i = 0; i < _MAP_HEIGHT_; i++)
	{
		/* Outside(border) of Map */
		strcat(buffer, "!!");

		/* Inside */
		for (int j = 0; j < _MAP_WIDTH_; j++)
		{
			if (EqualsWithPlayerPos(j, i))
			{
				strcat(buffer, "()");
				continue;
			}

			switch (Map[i][j])
			{
			case _NONE_:
				strcat(buffer, "  ");
				break;
			case _BLOCK_:
				strcat(buffer, "[]");
				break;
			case _HOUSE_:
				strcat(buffer, "{}");
				break;
			case _BOMB_:
				strcat(buffer, "<>");
			default:
				break;
			}
		}

		/* Outside(border) of Map */
		strcat(buffer, "!!");

		strcat(buffer, "\n");
	}

	/* Outside(border) of Map */
	for (int i = 0; i < _MAP_WIDTH_+2; i++)
	{
		strcat(buffer, "!!");
	}
	strcat(buffer, "\n");
}

void displayGame()
{
	char buffer[_SCREEN_WIDTH_ * _SCREEN_HEIGHT_];
	char input;
	
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

