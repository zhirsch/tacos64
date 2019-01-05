#include <kprintf.h>

void kernel_main(void) {
  kprintf("TacOS%d", 64);
  while (1) {
    __asm__ ("cli; hlt");
  }
  // TODO
}
