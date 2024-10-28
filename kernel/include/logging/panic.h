#pragma once

extern void panic(const char* fmt, ...) __attribute__((format(printf, 1, 2), noreturn));