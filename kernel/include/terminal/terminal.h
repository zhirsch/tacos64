#pragma once

extern void clear_screen();

extern void kprintf(const char* fmt, ...)  __attribute__ ((format (printf, 1, 2)));