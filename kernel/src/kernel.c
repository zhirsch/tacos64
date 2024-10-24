#include "cpu/apic.h"
#include "cpu/idt.h"
#include "cpu/tss.h"
#include "string/snprintf.h"
#include "terminal/terminal.h"

void kernel_main(unsigned int multiboot2_magic, unsigned int multiboot2_info) {
  clear_screen();
  kprintf("Welcome to TacOS64!\n");
  kprintf("Multiboot2 magic=0x%x info=0x%x\n", multiboot2_magic, multiboot2_info);

  kprintf("Initializing the tss\n");
  tss_initialize();

  kprintf("Initializing the apic\n");
  apic_initialize();

  kprintf("Initializing the idt\n");
  idt_initialize();

  kprintf("Enabling interrupts\n");
  __asm__("sti");
}
