#pragma once

#include <stdarg.h>

extern int snprintf(char* str, unsigned int size, const char* format, ...);
extern int vsnprintf(char* str, unsigned int size, const char* format, va_list ap);
