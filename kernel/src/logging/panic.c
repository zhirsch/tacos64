#include "logging/panic.h"

#include <stdarg.h>

#include "string/snprintf.h"
#include "terminal/terminal.h"

extern void halt(void) __attribute__((noreturn));

void panic(const char* fmt, ...) {
  char str[128];

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(str, 127, fmt, ap);
  va_end(ap);

  kprintf("PANIC! %s", str);
  halt();
}