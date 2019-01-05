#include <asm/multiboot/multiboot2.h>
#include <portio.h>

#define assert(pred, msg) if (!(pred)) { panic(msg); }
#define hlt() while (1) { __asm__ volatile ("cli; hlt"); }

static void bochs_write(const char* msg) {
  for (const char* p = msg; *p != '\0'; p++) {
    outb(0xE9, *p);
  }
}

static void panic(const char* msg) {
  bochs_write("PANIC: ");
  bochs_write(msg);
  bochs_write("\n");
  hlt();
}

void kernel_main(unsigned long multiboot2_magic, unsigned long multiboot2_addr) {
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

  assert(multiboot2_magic == MULTIBOOT2_BOOTLOADER_MAGIC, "Multiboot2 magic BAD");
  (void)multiboot2_addr;

  hlt();
}
