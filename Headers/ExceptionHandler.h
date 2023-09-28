#pragma once

typedef enum ExceptionType
{
	_UNKNOWN_EXCEPTION_,
	_STAGE_FILE_NOT_FOUND_,
} ExceptionType;

void throwFatalException(ExceptionType);
