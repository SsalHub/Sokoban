#pragma once

#include "BaseData.h"

void playStage();
int loadStageSelect(int);
Flag playGame(MapData*);
Flag confirmRestartStage();
int pauseGame();
void stageClear(int);
