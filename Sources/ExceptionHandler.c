
#include "../Headers/ExceptionHandler.h"

void throwFatalException(ExceptionType e)
{
	switch (e)
	{
		case _UNKNOWN_EXCEPTION_:
			break;
		case _STAGE_FILE_NOT_FOUND_:
			printf("Fatal Exception : Failed to load stage data!\nEnd console game progresses.\n");
			exit(1);
	}
}
