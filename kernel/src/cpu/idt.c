#include "cpu/idt.h"

#include <stdint.h>

#include "terminal/terminal.h"

extern void idt_load(void);
extern void (*(isr_array[]))(void);

extern struct {
  unsigned int offset_1   : 16;
  unsigned int selector   : 16;
  unsigned int ist        :  3;
  unsigned int reserved_1 :  5;
  unsigned int type       :  4;
  unsigned int reserved_2 :  1;
  unsigned int dpl        :  2;
  unsigned int present    :  1;
  unsigned int offset_2   : 16;
  unsigned int offset_3   : 32;
  unsigned int reserved_3 : 32;
} __attribute__((packed)) idt[];

static void set_idt_entry(int index);

void idt_initialize(void) {
  for (int i = 0; i < 256; i++) {
    set_idt_entry(i);
  }
  idt_load();
}

void isr_common(int vector, long errorcode) {
  kprintf("ISR %02x (errorcode=%lx)\n", vector, errorcode);
  while (1) {
    __asm__("cli; hlt");
  }
}

static void set_idt_entry(int index) {
  const uintptr_t isr_addr = (uintptr_t)(isr_array[index]);
  idt[index].offset_1 = (isr_addr & 0x000000000000ffff) >> 0;
  idt[index].selector = 0x08;
  idt[index].ist = 0;
  idt[index].reserved_1 = 0;
  idt[index].type = 0xe;
  idt[index].reserved_2 = 0;
  idt[index].dpl = 0;
  idt[index].present = 1;
  idt[index].offset_2 = (isr_addr & 0x00000000ffff0000) >> 16;
  idt[index].offset_3 = (isr_addr & 0xffffffff00000000) >> 32;
  idt[index].reserved_3 = 0;
}