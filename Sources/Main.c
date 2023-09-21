#include "BaseData.h"
#include "DisplayGame.h"

int main()
{
	initGame();
	initMap();
	setPlayerPos(0, 0);

	displayGame();
	return 0;
}
