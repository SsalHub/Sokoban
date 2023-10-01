#include "../Headers/ScreenBuffer.h"
#include "../Headers/StageSelect.h"

void loadMainMenu()
{
	renderMainMenuScreen();
	printMainMenuScreen();
//	showMainMenuScreen();
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
	
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	sprintf(content, "Press Any Key");
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	
	/* Fill screen with background color. */
	fillColorToScreen(bColor, tColor);
	
	/* Print title and content. */
	renderToCurrentScreen(title, titlePos, bColor, tColor);
	renderToCurrentScreen(content, contentPos, bColor, tColor);
	
//	_beginthreadex(NULL, 0, showBlinkingString, &contentData, 0, NULL);
}
