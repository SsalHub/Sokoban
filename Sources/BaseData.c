#define _CRT_SECURE_NO_WARNINGS
#include "BaseData.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


HANDLE screenBuffer[2];
int currentScreenBufferIndex;
int Map[_MAP_HEIGHT_][_MAP_WIDTH_];
Position player;

void initGame()
{
	char screenInitCommand[50] = "";
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	system(screenInitCommand);
	
	screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	currentScreenBufferIndex = 0;

	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(screenBuffer[0], &cursor);
	SetConsoleCursorInfo(screenBuffer[1], &cursor);
}

void initMap()
{
	for (int i = 0; i < _MAP_HEIGHT_; i++)
	{
		for (int j = 0; j < _MAP_WIDTH_; j++)
		{
			Map[i][j] = _NONE_;
		}
	}

	Map[2][3] = _BLOCK_;
	Map[3][4] = _HOUSE_;
	Map[4][5] = _BOMB_;
}

void setPlayerPos(int x, int y)
{
	player.x = x;
	player.y = y;
}

/* Translate player's position. If success it returns true, or false. */
bool translatePlayerPos(int x, int y)
{
	if (player.x + x < 0 || _MAP_WIDTH_ <= player.x + x)
		return false;
	if (player.y + y < 0 || _MAP_HEIGHT_ <= player.y + y)
		return false;
	player.x += x;
	player.y += y;
	return true;
}

bool EqualsWithPlayerPos(int x, int y)
{
	if (player.x == x && player.y == y)
		return true;
	return false;
}

void clearScreen()
{
	COORD pos = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(screenBuffer[currentScreenBufferIndex], ' ', _SCREEN_WIDTH_ * _SCREEN_HEIGHT_, pos, &dw);
}

void exitGame()
{
	system("cls");
	printf("THANKS FOR PLAYING\n\n\n\n\n\n\n");
	exit(0);
}

void showRedEffect()
{
	setConsoleColor(_RED_, _WHITE_);
	Sleep(10); // 0.05sec
	setConsoleColor(_BLACK_, _WHITE_);
}

void printScreen(char* s)
{
	COORD pos = { 0, 0 };
	DWORD dw;
	SetConsoleCursorPosition(screenBuffer[currentScreenBufferIndex], pos);
	WriteFile(screenBuffer[currentScreenBufferIndex], s, strlen(s), &dw, NULL);
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
	currentScreenBufferIndex = !currentScreenBufferIndex;
}

void releaseScreen()
{
	CloseHandle(screenBuffer[0]);
	CloseHandle(screenBuffer[1]);
}

void setConsoleColor(int background, int text)
{
	HANDLE tempBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTextAttribute(tempBuffer, (background << 4) + text);
	SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
}
