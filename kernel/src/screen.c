#include "screen.h"

#include <stdarg.h>

#include "snprintf.h"
#include "tacos64/string.h"

#define COLS 80
#define ROWS 25

static volatile char* const video = (volatile char*) 0xB8000;

static int cursor_row = 0;
static int cursor_col = 0;

static void putch(char ch);

void screen_clear(void) {
  memset((void*)video, 0, 2 * ROWS * COLS);
}

void screen_printf(const char* fmt, ...) {
  char p[256];
  va_list ap;
  int n;

  va_start(ap, fmt);
  n = vsnprintf(p, 256 - 1, fmt, ap);
  va_end(ap);

  for (int i = 0; i < n; i++) {
    putch(p[i]);
  }
}

static void putch(char ch) {
  video[2 * (cursor_row * COLS + cursor_col) + 0] = ch;
  video[2 * (cursor_row * COLS + cursor_col) + 1] = 0x1f;

  cursor_col++;
  if (cursor_col == COLS) {
    cursor_col = 0;
    cursor_row++;
  }
  if (cursor_row == ROWS) {
    // TODO: Scroll screen.
    cursor_row = 0;
  }
}
