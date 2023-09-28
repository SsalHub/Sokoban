#include <stdio.h>
#include <Windows.h>

#include "../Headers/BaseData.h"
#include "../Headers/ScreenBuffer.h"

int screenBufferLength;
ScreenBuffer screenBuffer[6];

ScreenBuffer* getScreenBuffer(ScreenBufferIndex i)
{
	return screenBuffer + i;
}

void initEffectScreen()
{
	ConsoleColor black = _BLACK_, red = _BRIGHTRED_, white = _WHITE_;
	ScreenBuffer* effectScreen = getScreenBuffer(_EFFECT_SCREEN_);
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[_SCREEN_WIDTH_*_SCREEN_HEIGHT_+1];
	int i, j;
	
	bufferString[0] = '\0';
	SetConsoleTextAttribute(effectScreen->buffer[effectScreen->currentIndex], white | (red << 4));
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		for (j = 0; j < _SCREEN_WIDTH_; j++)
		{
			strcat(bufferString, " ");
		}
		strcat(bufferString, "\n");
	}
	SetConsoleCursorPosition(effectScreen->buffer[effectScreen->currentIndex], pos);
	WriteFile(effectScreen->buffer[effectScreen->currentIndex], bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(effectScreen->buffer[effectScreen->currentIndex], white | (black << 4));
}

void initStageRestartScreen()
{
	ConsoleColor black = _BLACK_, green = _BRIGHTGREEN_, white = _WHITE_;
	ScreenBuffer* restartScreen = getScreenBuffer(_STAGE_RESTART_SCREEN_);
	COORD pos = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char title[64] 		= "";
	char content[64] 	= "";
	int i, j, tlen, clen;
	
	sprintf(title, "이 스테이지를 처음부터 다시 진행하시겠습니까?");
	sprintf(content, "← 돌아가기        처음부터 다시하기 →");
	tlen = strlen(title);
	clen = strlen(content);
	bufferString[0] = '\0';
	SetConsoleTextAttribute(restartScreen->buffer[restartScreen->currentIndex], black | (green << 4));
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
	SetConsoleCursorPosition(restartScreen->buffer[restartScreen->currentIndex], pos);
	WriteFile(restartScreen->buffer[restartScreen->currentIndex], bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleTextAttribute(restartScreen->buffer[restartScreen->currentIndex], white | (black << 4));
}

void swapScreenIndex(ScreenBuffer* sb)
{
	sb->currentIndex = !(sb->currentIndex);
}

void printRenderedScreen(ScreenBuffer* sb, char* bufferString)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	
	SetConsoleCursorPosition(sb->buffer[sb->currentIndex], pos);
	WriteFile(sb->buffer[sb->currentIndex], bufferString, strlen(bufferString), &dw, NULL);
	SetConsoleActiveScreenBuffer(sb->buffer[sb->currentIndex]);
	swapScreenIndex(sb);
}

void clearScreen(ScreenBuffer* sb)	
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(sb->buffer[sb->currentIndex], ' ', _SCREEN_WIDTH_ * _SCREEN_HEIGHT_, pos, &dw);
}

void showRedEffect(ScreenBuffer* sb)
{
	SetConsoleActiveScreenBuffer(screenBuffer[_EFFECT_SCREEN_].buffer[0]);
	Sleep(10); // 0.05sec
	SetConsoleActiveScreenBuffer(sb->buffer[sb->currentIndex]);
}

void printScreen(ScreenBuffer* sb, char* s)
{
	ConsoleColor white = _WHITE_, yellow = _BRIGHTYELLOW_, black = _BLACK_;
	COORD playerPos = { ((int)(_SCREEN_WIDTH_ * 0.5) - mapData.width + (player.x * 2)), (_SCREEN_HEIGHT_ - mapData.height) * 0.5 + player.y };
	COORD zero = { 0, 0 };
	DWORD dw;
	char playerCharacter[5] = "";
	
	/* Write rendered stage(= map) data to HANDLE(= screen buffer). */
	SetConsoleCursorPosition(sb->buffer[sb->currentIndex], zero);
	WriteFile(sb->buffer[sb->currentIndex], s, strlen(s), &dw, NULL);
	
	/* Write only player's character with yellow color to HANDLE(= screen buffer). */
	SetConsoleCursorPosition(sb->buffer[sb->currentIndex], playerPos);
	SetConsoleTextAttribute(sb->buffer[sb->currentIndex], yellow | (black << 4));
	sprintf(playerCharacter, "★");
	WriteFile(sb->buffer[sb->currentIndex], playerCharacter, strlen(playerCharacter), &dw, NULL);
	SetConsoleTextAttribute(sb->buffer[sb->currentIndex], white | (black << 4));
	
	SetConsoleActiveScreenBuffer(sb->buffer[sb->currentIndex]);
	swapScreenIndex(sb);
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
	int i;
	
	for (i=0; i < screenBufferLength; i++)
	{
		CloseHandle(screenBuffer[i].buffer[0]);
		CloseHandle(screenBuffer[i].buffer[1]);
	}
}

void showStageClearScreen(int stageIndex)
{
	ConsoleColor black = _BLACK_, yellow = _BRIGHTYELLOW_, white = _WHITE_;
	ScreenBuffer* clearScreen = getScreenBuffer(_STAGE_CLEAR_SCREEN_);
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char stageClearText[_SCREEN_WIDTH_+1];
	int i, j, loop, idx, bufferIndex = 0, slen;
	
	SetConsoleTextAttribute(clearScreen->buffer[0], black | (yellow << 4));
	SetConsoleTextAttribute(clearScreen->buffer[1], black | (yellow << 4));
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
		printRenderedScreen(clearScreen, bufferString);
		Sleep(300);	// 0.3sec
	}
	SetConsoleTextAttribute(clearScreen->buffer[0], white | (black << 4));
	SetConsoleTextAttribute(clearScreen->buffer[1], white | (black << 4));
}

void showLoadingStageScreen(int stageIndex)
{
	ConsoleColor black = _BLACK_, skyblue = _SKYBLUE_, white = _WHITE_;
	ScreenBuffer* loadingScreen = getScreenBuffer(_STAGE_LOADING_SCREEN_);
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char loadingText[_SCREEN_WIDTH_+1];
	int i, j, loop, idx, bufferIndex = 0, slen;
	
	SetConsoleTextAttribute(loadingScreen->buffer[0], black | (skyblue << 4));
	SetConsoleTextAttribute(loadingScreen->buffer[1], black | (skyblue << 4));
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
		printRenderedScreen(loadingScreen, bufferString);
		Sleep(300);	// 0.3sec
	}
	SetConsoleTextAttribute(loadingScreen->buffer[0], white | (black << 4));
	SetConsoleTextAttribute(loadingScreen->buffer[1], white | (black << 4));
}

Flag showStageRestartScreen()
{
	char input;
	
	SetConsoleActiveScreenBuffer(screenBuffer[_STAGE_RESTART_SCREEN_].buffer[0]);
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
