#include <screen.h>

void kernel_main(void) {
  screen_clear();
  screen_printf("%s%d by zhirsch/Moldy", "TacOS", 64);

  while (1) {
    __asm__ ("cli; hlt");
  }
  // TODO
}
