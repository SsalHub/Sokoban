#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"

ScreenBuffer screenBuffer;

void swapScreenIndex()
{
	screenBuffer.currentIndex = !(screenBuffer.currentIndex);
}

//void initEffectScreen()
//{
//	ConsoleColor black = _BLACK_, red = _BRIGHTRED_, white = _WHITE_;
//	COORD pos = { 0, 0 };
//	DWORD dw;
//	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
//	int i, j;
//	
//	bufferString[0] = '\0';
//	SetConsoleTextAttribute(effectScreen->buffer[effectScreen->currentIndex], white | (red << 4));
//	for (i = 0; i < _SCREEN_HEIGHT_; i++)
//	{
//		for (j = 0; j < _SCREEN_WIDTH_; j++)
//		{
//			strcat(bufferString, " ");
//		}
//		strcat(bufferString, "\n");
//	}
//	SetConsoleCursorPosition(effectScreen->buffer[effectScreen->currentIndex], pos);
//	WriteFile(effectScreen->buffer[effectScreen->currentIndex], bufferString, strlen(bufferString), &dw, NULL);
//	SetConsoleTextAttribute(effectScreen->buffer[effectScreen->currentIndex], white | (black << 4));
//}

//void initStageRestartScreen()
//{
//	ConsoleColor black = _BLACK_, green = _BRIGHTGREEN_, white = _WHITE_;
//	ScreenBuffer* restartScreen = getScreenBuffer(_STAGE_RESTART_SCREEN_);
//	COORD pos = { 0, 0 };
//	DWORD dw;
//	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
//	char title[64] 		= "";
//	char content[64] 	= "";
//	int i, j, tlen, clen;
//	
//	sprintf(title, "이 스테이지를 처음부터 다시 진행하시겠습니까?");
//	sprintf(content, "← 돌아가기        처음부터 다시하기 →");
//	tlen = strlen(title);
//	clen = strlen(content);
//	bufferString[0] = '\0';
//	SetConsoleTextAttribute(restartScreen->buffer[restartScreen->currentIndex], black | (green << 4));
//	for (i = 0; i < _SCREEN_HEIGHT_; i++)
//	{
//		if (i == ((int)(_SCREEN_HEIGHT_ * 0.5) - 2))
//		{
//			for (j = 0; j < (int)((_SCREEN_WIDTH_-tlen)*0.5); j++)
//			{
//				strcat(bufferString, " ");
//			}
//			strcat(bufferString, title);
//			for (j = 0; j < (int)((_SCREEN_WIDTH_-tlen)*0.5+1); j++)
//			{
//				strcat(bufferString, " ");
//			}
//		}
//		else if (i == ((int)(_SCREEN_HEIGHT_ * 0.5) + 2))
//		{
//			for (j = 0; j < (int)((_SCREEN_WIDTH_-clen)*0.5); j++)
//			{
//				strcat(bufferString, " ");
//			}
//			strcat(bufferString, content);
//			for (j = 0; j < (int)((_SCREEN_WIDTH_-clen)*0.5+1); j++)
//			{
//				strcat(bufferString, " ");
//			}
//		}
//		else
//		{
//			for (j = 0; j < _SCREEN_WIDTH_; j++)
//			{
//				strcat(bufferString, " ");
//			}
//		}
//		strcat(bufferString, "\n");
//	}
//	SetConsoleCursorPosition(restartScreen->buffer[restartScreen->currentIndex], pos);
//	WriteFile(restartScreen->buffer[restartScreen->currentIndex], bufferString, strlen(bufferString), &dw, NULL);
//	SetConsoleTextAttribute(restartScreen->buffer[restartScreen->currentIndex], white | (black << 4));
//}

//void printScreen(char* s, ConsoleColor bColor, ConsoleColor tColor)
//{
//	COORD playerPos = { ((int)(_SCREEN_WIDTH_ * 0.5) - mapData.width + (player.x * 2)), (_SCREEN_HEIGHT_ - mapData.height) * 0.5 + player.y };
//	COORD zero = { 0, 0 };
//	DWORD dw;
//	char playerCharacter[5] = "";
//	
//	clearScreen();
//	
//	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
//	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
//	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
//	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], s, strlen(s), &dw, NULL);
//	
//	/* Write only player's character with yellow color to HANDLE(= screen buffer). */
//	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], playerPos);
//	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
//	sprintf(playerCharacter, "★");
//	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
//	
//	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
//	swapScreenIndex();
//}

void printStageScreen(char* map, ConsoleColor bColor, ConsoleColor tColor)
{
	int centerAlignX = (int)(((_SCREEN_WIDTH_)-((mapData.width+2)*2))*0.5), centerAlignY = (int)((_SCREEN_HEIGHT_ - mapData.height)*0.5)-1;
	COORD centeredMapPos = { centerAlignX, centerAlignY }, playerPos = { centeredMapPos.X+((player.x+1)*2), centeredMapPos.Y+player.y+1 };
	COORD zero = { 0, 0 };
	DWORD dw;
	char playerCharacter[5] = "";
	char* nextLine;
	
	clearScreen();
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	nextLine = strtok(map, "\n");
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
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	sprintf(playerCharacter, "★");
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
	
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
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

//void printPlayer(ScreenBuffer* sb)
//{
//	ConsoleColor white = _WHITE_, yellow = _BRIGHTYELLOW_, black = _BLACK_;
//	COORD playerPos = { ((int)(_SCREEN_WIDTH_ * 0.5) - mapData.width + (player.x * 2)), (_SCREEN_HEIGHT_ - mapData.height) * 0.5 + player.y };
//	DWORD dw;
//	char playerCharacter[5] = "";
//	
//	SetConsoleCursorPosition(sb->buffer[sb->currentIndex], playerPos);
//	SetConsoleTextAttribute(sb->buffer[sb->currentIndex], yellow | (black << 4));
//	sprintf(playerCharacter, "★");
//	WriteFile(sb->buffer[sb->currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
//	SetConsoleTextAttribute(sb->buffer[sb->currentIndex], white | (black << 4));
//}

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
		printRenderedScreen(bufferString, _BRIGHTYELLOW_, _BLACK_);
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
	printRenderedScreen(bufferString, _BRIGHTGREEN_, _BLACK_);
	
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

void showRedEffectScreen()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	int i, j;
	
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	printRenderedScreen(bufferString, _BRIGHTRED_, _WHITE_);
	Sleep(10); // 0.05sec
}
