#include <stdarg.h>

#include <kprintf.h>

#define MAXBUFSIZ 1024

static volatile char* g_video = (volatile char*)0xB8000;
static int g_video_pos = 0;

int kprintf(const char* fmt, ...) {
  char buffer[MAXBUFSIZ];

  va_list args;
  va_start(args, fmt);
  int ret = vsnprintf(buffer, MAXBUFSIZ, fmt, args);
  va_end(args);

  if (ret <= 0) {
    return ret;
  }

  for (int i = 0; i < ret; i++) {
    kputchar(buffer[i]);
  }
  
  return ret;
}

void kputchar(char ch) {
  g_video[g_video_pos++] = ch;
  g_video[g_video_pos++] = 0x1F;
}
