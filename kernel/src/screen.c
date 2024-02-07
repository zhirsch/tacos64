#include <screen.h>

#include <tacos64/string.h>

#define COLS 80
#define ROWS 25

static volatile char* const video = (volatile char*) 0xB8000;

void screen_clear(void) {
  memset((void*)video, 0, 2 * ROWS * COLS);
}
