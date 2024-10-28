#include "terminal/terminal.h"

#include <stdarg.h>

#include "string/snprintf.h"

#define MAXROWS 25
#define MAXCOLS 80
#define VIDEO ((volatile char*)0xB8000)

static int cursor_row = 0;
static int cursor_col = 0;

static void print_char_at(char ch, int row, int col) {
  const int i = 2 * (col + (MAXCOLS * row));
  VIDEO[i + 0] = ch;
  VIDEO[i + 1] = 0x07;
}

static void print_char(char ch) {
  if (ch != '\n') {
    print_char_at(ch, cursor_row, cursor_col++);
  }
  if (cursor_col == MAXCOLS || ch == '\n') {
    cursor_row++;
    cursor_col = 0;
  }
}

void clear_screen() {
  cursor_row = 0;
  cursor_col = 0;
  for (int row = 0; row < MAXROWS; row++) {
    for (int col = 0; col < MAXCOLS; col++) {
      print_char_at(' ', row, col);
    }
  }
}

void kprintf(const char* fmt, ...) {
  int total;
  char str[128];

  va_list ap;
  va_start(ap, fmt);
  total = vsnprintf(str, 127, fmt, ap);
  va_end(ap);

  for (int i = 0; i < total; i++) {
    print_char(str[i]);
  }
}