#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/ExceptionHandler.h"

ScreenBuffer screenBuffer;

void swapScreenIndex()
{
	screenBuffer.currentIndex = !(screenBuffer.currentIndex);
}

void renderStageMapScreen()
{
	ConsoleColor bColor = _DARK_PURPLE_, tColor = _BLUE_, tPlayerColor = _YELLOW_;
	int centerAlignX = (int)(((_SCREEN_WIDTH_)-((mapData.width+2)*2))*0.5), centerAlignY = (int)((_SCREEN_HEIGHT_ - mapData.height)*0.5)-1;
	COORD centeredMapPos = { centerAlignX, centerAlignY }, playerPos = { centeredMapPos.X+((player.x+1)*2), centeredMapPos.Y+player.y+1 };
	COORD zero = { 0, 0 };
	DWORD dw;
	char stageMapString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char block[3] = "□", playerCharacter[5] = "★";
	char* nextLine;
	int i, j;

	clearScreen();
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	/* Outside(border) of Map */
	stageMapString[0] = '\0';
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, block);
	}
	strcat(stageMapString, "\n");
	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(stageMapString, block);
		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{
			/* Player */
			if (EqualsWithPlayerPos(j, i))
			{
				strcat(stageMapString, "☆");
				continue;
			}
			/* GameObjects */
			switch (mapData.map[i][j])
			{
				case _NONE_:
					strcat(stageMapString, "  ");
					break;
				case _BLOCK_:
					strcat(stageMapString, block);
					break;
				case _BALL_:
					strcat(stageMapString, "◎");
					break;
				case _EMPTY_BOX_:
					strcat(stageMapString, "♤");
					break;
				case _FILLED_BOX_:
					strcat(stageMapString, "♠");
					break;
				default:
					break;
			}
		}
		/* Outside(border) of Map */
		strcat(stageMapString, block);
		strcat(stageMapString, "\n");
	}
	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(stageMapString, block);
	}
	strcat(stageMapString, "\n");
	
	/* Write stage map string to screen buffer. */
	nextLine = strtok(stageMapString, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], centeredMapPos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		centeredMapPos.Y++;
	}
	centeredMapPos.X = centerAlignX;
	centeredMapPos.Y = centerAlignY;
	
	/* Write only player's character with yellow color to HANDLE(= screen buffer). */
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], playerPos);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tPlayerColor | (bColor << 4));
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
}

void renderToCurrentScreen(char* str, COORD pos, ConsoleColor bColor, ConsoleColor tColor)
{
	DWORD dw;
	char* nextLine;
	
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
}

void renderRedEffectScreen()
{
	ConsoleColor bColor = _RED_, tColor = _WHITE_;
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	int i, j;
	
	clearScreen();
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}

/* Not swapping screen buffer. */
void printScreen()
{
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
}

void printMainMenuScreen()
{
	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}
}

void printRenderedScreen(char* bufferString, ConsoleColor bColor, ConsoleColor tColor)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	
	clearScreen();
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
	
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void clearScreen()	
{
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	int i, j;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], _WHITE_ | (_BLACK_ << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}

void releaseScreen()
{
	CloseHandle(screenBuffer.buffer[0]);
	CloseHandle(screenBuffer.buffer[1]);
}

void showStageClearScreen(int stageIndex)
{
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char stageClearText[_SCREEN_WIDTH_+1];
	int i, j, loop, idx, bufferIndex = 0, slen;
	
	sprintf(stageClearText, "Stage%02d Clear", stageIndex);
	for (loop = 0; loop < 5; loop++)
	{
		strcat(stageClearText, " !");
		slen = strlen(stageClearText);
		bufferString[0] = '\0';
		for (i = 0; i < _SCREEN_HEIGHT_ ; i++)
		{
			if (i == (int)(_SCREEN_HEIGHT_ * 0.5))
			{
				for (j = 0; j < (int)((_SCREEN_WIDTH_ - slen) * 0.5); j++)
				{
					strcat(bufferString, " ");
				}
				strcat(bufferString, stageClearText);
				for (j = 0; j < (int)((_SCREEN_WIDTH_ - slen) * 0.5) + 1; j++)
				{
					strcat(bufferString, " ");
				}
			}
			else
			{
				for (j = 0; j < _SCREEN_WIDTH_; j++)
				{
					strcat(bufferString, " ");
				}
			}
			strcat(bufferString, "\n");
		}
		printRenderedScreen(bufferString, _LIGHT_YELLOW_, _BLACK_);
		Sleep(300);	// 0.3sec
	}
}

void showLoadingStageScreen(int stageIndex)
{
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char loadingText[_SCREEN_WIDTH_+1];
	int i, j, loop, idx, bufferIndex = 0, slen;
	
	sprintf(loadingText, "Stage%02d Loading", stageIndex);
	for (loop = 0; loop < 3; loop++)
	{
		strcat(loadingText, " .");
		slen = strlen(loadingText);
		bufferString[0] = '\0';
		for (i = 0; i < _SCREEN_HEIGHT_ ; i++)
		{
			/* Print loading text on center of screen. */
			if (i == (int)(_SCREEN_HEIGHT_ * 0.5))
			{
				for (j = 0; j < (int)((_SCREEN_WIDTH_ - slen) * 0.5); j++)
				{
					strcat(bufferString, " ");
				}
				strcat(bufferString, loadingText);
				for (j = 0; j < (int)((_SCREEN_WIDTH_ - slen) * 0.5) + 1; j++)
				{
					strcat(bufferString, " ");
				}
			}
			else
			{
				for (j = 0; j < _SCREEN_WIDTH_; j++)
				{
					strcat(bufferString, " ");
				}
			}
			strcat(bufferString, "\n");
		}
		printRenderedScreen(bufferString, _SKYBLUE_, _BLACK_);
		Sleep(300);	// 0.3sec
	}
}

Flag showStageRestartScreen()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char title[64] 		= "";
	char content[64] 	= "";
	char input;
	int i, j, tlen, clen;
	
	sprintf(title, "이 스테이지를 처음부터 다시 진행하시겠습니까?");
	sprintf(content, "← 돌아가기        |        처음부터 다시하기 →");
	tlen = strlen(title);
	clen = strlen(content);
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		if (i == ((int)(_SCREEN_HEIGHT_ * 0.5) - 2))
		{
			for (j = 0; j < (int)((_SCREEN_WIDTH_-tlen)*0.5); j++)
			{
				strcat(bufferString, " ");
			}
			strcat(bufferString, title);
			for (j = 0; j < (int)((_SCREEN_WIDTH_-tlen)*0.5+1); j++)
			{
				strcat(bufferString, " ");
			}
		}
		else if (i == ((int)(_SCREEN_HEIGHT_ * 0.5) + 2))
		{
			for (j = 0; j < (int)((_SCREEN_WIDTH_-clen)*0.5); j++)
			{
				strcat(bufferString, " ");
			}
			strcat(bufferString, content);
			for (j = 0; j < (int)((_SCREEN_WIDTH_-clen)*0.5+1); j++)
			{
				strcat(bufferString, " ");
			}
		}
		else
		{
			for (j = 0; j < _SCREEN_WIDTH_; j++)
			{
				strcat(bufferString, " ");
			}
		}
		strcat(bufferString, "\n");
	}
	printRenderedScreen(bufferString, _GREEN_, _BLACK_);
	
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _ESCAPE_:
				case _LEFT_:
					return _FALSE_;
				case _RIGHT_:
					return _STAGE_RESTART_;
				default:
					break;
			}
		}
	}
	return _FALSE_;
}

int showStageSelectScreen(int maxStage, int currentStage)
{
	ConsoleColor bColor = _OCEAN_BLUE_, tColor = _BLACK_, tSelectedColor = _RED_, tInputColor = _YELLOW_;
	int stageSelectBoxY = (int)(_SCREEN_HEIGHT_ * 0.3), stageStructureY = (int)(_SCREEN_HEIGHT_ * 0.6);
	COORD leftArrowPos = { (int)(_SCREEN_WIDTH_ * 0.2), stageSelectBoxY }, rightArrowPos = { (int)(_SCREEN_WIDTH_ * 0.8), stageSelectBoxY };
	COORD stageStructurePos = { 0, stageStructureY }, zero = { 0, 0 };
	DWORD dw;
	StageSelectBox stageSelectBox[3];
	char leftArrowStr[3] = "←", rightArrowStr[3] = "→"; 
	char stageStructure[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	char input;
	int i, j, currentChangedIndex = currentStage;
	
	
	while (1)
	{
		
		/* Initialize stage select box. */
		for (i = 0; i < 3; i++)
		{
			stageSelectBox[i].stageIndex = i + (-1 + currentChangedIndex);
			if (stageSelectBox[i].stageIndex < 1 || maxStage < stageSelectBox[i].stageIndex)
				continue;
			
			sprintf(stageSelectBox[i].buffer,"┌───┐\n");
			j = (int)(strlen(stageSelectBox[i].buffer) * 1.5);
			sprintf(stageSelectBox[i].buffer,"┌───┐\n│%03d│\n└───┘\n", stageSelectBox[i].stageIndex);
			stageSelectBox[i].pos.X = (int)(_SCREEN_WIDTH_ * 0.5) + ((-1 + i) * j);
			stageSelectBox[i].pos.Y = stageSelectBoxY;
		}
		
		fillColorToScreen(bColor, tColor);
		
		/* Print each arrows. */
		renderToCurrentScreen(leftArrowStr, leftArrowPos, bColor, tColor);
		renderToCurrentScreen(rightArrowStr, rightArrowPos, bColor, tColor);
		
		/* Print Stage Select Boxes. */
		for (i = 0; i < 3; i++)
		{
			if (stageSelectBox[i].stageIndex < 1 || maxStage < stageSelectBox[i].stageIndex)
				continue;
				
			if (i == 1)
				renderToCurrentScreen(stageSelectBox[i].buffer, stageSelectBox[i].pos, bColor, tSelectedColor);
			else
				renderToCurrentScreen(stageSelectBox[i].buffer, stageSelectBox[i].pos, bColor, tColor);
		}
		
//		/* Print currently selected stage's map structure. */
//		loadMapData(currentChangedIndex);
//		renderStageMap(stageStructure);
//		stageStructurePos.X = (int)((_SCREEN_WIDTH_ - (mapData.width + 2) * 2) * 0.5);
//		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], stageStructurePos);
//		SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
//		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], stageStructure, strlen(stageStructure), &dw, NULL);
//		
		/* Print actual buffer and swap */
		SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
		swapScreenIndex();
		
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _ESCAPE_:
				case _LEFT_:
					if (currentChangedIndex - 1 < 1) break;
					renderToCurrentScreen(leftArrowStr, leftArrowPos, bColor, tInputColor);
					currentChangedIndex = currentChangedIndex - 1;
					Sleep(100);	// 0.1sec
					break;
				case _RIGHT_:
					if (maxStage < currentChangedIndex + 1) break;
					renderToCurrentScreen(rightArrowStr, rightArrowPos, bColor, tInputColor);
					currentChangedIndex = currentChangedIndex + 1;
					Sleep(100);	// 0.1sec
					break;
				case _SPACE_:
				case _CARRIGE_RETURN_:
					/* select ok */
					return currentChangedIndex;
				default:
					break;
			}
		}
	}
	throwFatalException(_UNKNOWN_EXCEPTION_);
	return -1;
}

void showMainMenuScreen()
{
	ConsoleColor bColor = _GREEN_, tColor = _BLACK_;
	COORD titlePos = { 0, (int)(_SCREEN_HEIGHT_ * 0.3) }, contentPos = { 0, (int)(_SCREEN_HEIGHT_ * 0.75) };
	DWORD dw;
	BlinkingStringData contentData;
	char title[64], content[64];
	char bufferString[(_MAP_WIDTH_*2)*_MAP_HEIGHT_+1] = "";
	int i, j;
	
	sprintf(title, "Sokoban : 19 Song JaeUk in Hansung Univ.");
	titlePos.X = (int)((_SCREEN_WIDTH_ - strlen(title)) * 0.5);
	sprintf(content, "Press Any Key");
	contentPos.X = (int)((_SCREEN_WIDTH_ - strlen(content)) * 0.5);
	
	/* Fill screen with background color. */
	fillColorToScreen(bColor, tColor);
	
	/* Print title. */
	renderToCurrentScreen(title, titlePos, bColor, tColor);
	
	/* Begin thread to print 'press any key'. */
	contentData.str = content;
	contentData.lifetime = 1000;	// 1.0sec
	contentData.delay = 200;		// 0.2sec
	contentData.bColor = bColor;
	contentData.tColor = tColor;
	_beginthreadex(NULL, 0, showBlinkingString, &contentData, 0, NULL);
	
	while (1)
	{
		if (_kbhit())
		{
			return;
		}
	}
}

void showRedEffectScreen()
{
	renderRedEffectScreen();
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
	Sleep(10); // 0.05sec
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
{	
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1] = "";
	int i, j;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}
