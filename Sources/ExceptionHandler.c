
#include "../Headers/ExceptionHandler.h"

void throwFatalException(ExceptionType e)
{
	switch (e)
	{
		case _UNKNOWN_EXCEPTION_:
			printf("Fatal Exception : Unknown exception occurred while running this game.\nEnd console game progresses.\n");
			break;
		case _STAGE_FILE_NOT_FOUND_:
			printf("Fatal Exception : Failed to load stage data!\nEnd console game progresses.\n");
			break;
	}
	
	exit(1);
}
