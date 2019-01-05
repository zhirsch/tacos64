void kernel_main(void) {
  volatile char* video = (volatile char*)0xB8000;
  video[0] = 'T';
  video[1] = 0x1f;
  video[2] = 'a';
  video[3] = 0x1f;
  video[4] = 'c';
  video[5] = 0x1f;
  video[6] = 'O';
  video[7] = 0x1f;
  video[8] = 'S';
  video[9] = 0x1f;
  video[10] = '6';
  video[11] = 0x1f;
  video[12] = '4';
  video[13] = 0x1f;
  while (1) {
    __asm__ ("cli; hlt");
  }
  // TODO
}
