
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
	int stageIndex, stages;
	
	/* Some Stage selecting action */
	
	stages = countStage();
	stageIndex = 1;
	
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

void renderStageSelectBuffer(char* bufferString)
{
	
}
