void kernel_main(void) {
  while (1) {
    __asm__ ("cli; hlt");
  }
  // TODO
}
