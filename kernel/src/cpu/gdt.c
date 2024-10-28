#include "cpu/gdt.h"

#include <stdint.h>

extern union {
  struct {
    unsigned int limit_1  : 16;
    unsigned int base_1   : 16;
    unsigned int base_2   :  8;
    unsigned int access   :  8;
    unsigned int limit_2  :  4;
    unsigned int flags    :  4;
    unsigned int base_3   :  8;
  } __attribute__((packed)) entry;
  struct {
    unsigned int base     : 32;
    unsigned int reserved : 32;
  } __attribute__((packed)) tsshi;
} gdt[];

void gdt_set_entry(uintptr_t base, uint32_t limit, uint8_t access) {
  gdt[5].entry.limit_1  = (limit & 0x0000ffff) >> 0;
  gdt[5].entry.base_1   = (base & 0x000000000000ffff) >> 0;
  gdt[5].entry.base_2   = (base & 0x0000000000ff0000) >> 16;
  gdt[5].entry.access   = access;
  gdt[5].entry.limit_2  = (limit & 0x00ff0000) >> 16;
  gdt[5].entry.flags    = 0;
  gdt[5].entry.base_3   = (base & 0x00000000ff000000) >> 24;
  gdt[6].tsshi.base     = (base & 0xffffffff00000000) >> 32;
  gdt[6].tsshi.reserved = 0;
}