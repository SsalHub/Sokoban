
#include <Windows.h>
#include <direct.h>
#include <dirent.h>

#include "../Headers/BaseData.h"
#include "../Headers/DisplayGame.h"
#include "../Headers/ScreenBuffer.h"
#include "../Headers/ExceptionHandler.h"

void loadStageSelect()
{
	Flag flag;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	char input;
	int stageIndex, maxStage;
	
	/* Some Stage selecting action */
	
	maxStage = countStage();
	stageIndex = 1;
	stageIndex = showStageSelectScreen(maxStage, stageIndex);
	
	/* 할일 1출력 2입력받기 3받은입력검사 4검사후액션 */
//	while (1)
//	{
//		bufferString = '\0';
//		renderStageSelectBuffer(bufferString);
////		if (_kbhit())
////		{
////			input = _getch();
////			switch (input)
////			{
////				
//	}
	
	//something
	
	
	while(1)
	{
		showLoadingStageScreen(stageIndex);
		
		//Sleep(2000);	// 2.0sec
		
		loadMapData(stageIndex);
		flag = displayGame(stageIndex);
		
		switch (flag)
		{
			case _STAGE_CLEAR_:
				showStageClearScreen(stageIndex++);
				break;
			case _STAGE_RESTART_:
				/* Get back to loading screen without increasing stageIndex. */
				break;	
			default:
				break;
		}
	}
}

int countStage()
{
	char mapDataPath[1000] = "";
    struct dirent *dir;
    DIR *dp;
    int stageCount;

	_getcwd(mapDataPath, 1000);		// path of root of this project directory
	strcat(mapDataPath, "\\Maps\\");
	stageCount = 0;
	
	/* If stage map data dir not exists */
    if ((dp = opendir(mapDataPath)) == NULL)
    {
		throwFatalException(_STAGE_FILE_NOT_FOUND_);
    }

    while ((dir = readdir(dp)) != NULL)
    {
    	/* If found file is myself or parent dir */
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        stageCount++;
    }
    
    return stageCount;
}

void renderStageStruct(char* bufferString)
{
	int i, j;
	char block[3] = "□";

	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(bufferString, block);
	}
	strcat(bufferString, "\n");

	for (i = 0; i < mapData.height; i++)
	{
		/* Outside(border) of Map */
		strcat(bufferString, block);

		/* Inside */
		for (j = 0; j < mapData.width; j++)
		{

			/* GameObjects */
			switch (mapData.structure[i][j])
			{
				case _NONE_:
					strcat(bufferString, "  ");
					break;
					
				case _BLOCK_:
					strcat(bufferString, block);
					break;
					
				case _BALL_:
					strcat(bufferString, "◎");
					break;
					
				case _EMPTY_BOX_:
					strcat(bufferString, "♤");
					break;
					
				case _FILLED_BOX_:
					strcat(bufferString, "♠");
					break;
					
				default:
					break;
			}
		}

		/* Outside(border) of Map */
		strcat(bufferString, block);

		strcat(bufferString, "\n");
	}

	/* Outside(border) of Map */
	for (i = 0; i < mapData.width + 2; i++)
	{
		strcat(bufferString, block);
	}
	strcat(bufferString, "\n");
}
