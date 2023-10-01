#include <stdbool.h>
#include <conio.h>

#include "../Headers/ScreenBuffer.h"
#include "../Headers/StageSelect.h"

void loadMainMenu()
{
	renderMainMenuScreen();
	printScreen(renderMainMenuScreen, true);
	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}
	loadStageSelect();
}

void renderMainMenuScreen()
{
	ConsoleColor bColor = _DARK_PURPLE_, tColor = _BLUE_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.75) };
	DWORD dw;
	char title[64], content[64];
	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	int i, j;
	
	/* Fill screen with background color. */
	fillColorToScreen(bColor, tColor, false);
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], titlePos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], title, strlen(title), &dw, NULL);
	
	sprintf(content, "Press Any Key");
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], contentPos);	
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], content, strlen(content), &dw, NULL);
	
//	_beginthreadex(NULL, 0, showBlinkingString, &contentData, 0, NULL);
}
