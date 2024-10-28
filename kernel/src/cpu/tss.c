#include "cpu/tss.h"

#include <stdint.h>

#include "cpu/gdt.h"
#include "terminal/terminal.h"

extern void tss_load(void);

static struct tss {
  unsigned int reserved_1 : 32;
  unsigned int rsp0_lo    : 32;
  unsigned int rsp0_hi    : 32;
  unsigned int rsp1_lo    : 32;
  unsigned int rsp1_hi    : 32;
  unsigned int rsp2_lo    : 32;
  unsigned int rsp2_hi    : 32;
  unsigned int reserved_2 : 32;
  unsigned int reserved_3 : 32;
  unsigned int ist1_lo    : 32;
  unsigned int ist1_hi    : 32;
  unsigned int ist2_lo    : 32;
  unsigned int ist2_hi    : 32;
  unsigned int ist3_lo    : 32;
  unsigned int ist3_hi    : 32;
  unsigned int ist4_lo    : 32;
  unsigned int ist4_hi    : 32;
  unsigned int ist5_lo    : 32;
  unsigned int ist5_hi    : 32;
  unsigned int ist6_lo    : 32;
  unsigned int ist6_hi    : 32;
  unsigned int ist7_lo    : 32;
  unsigned int ist7_hi    : 32;
  unsigned int reserved_4 : 32;
  unsigned int reserved_5 : 32;
  unsigned int reserved_6 : 16;
  unsigned int iopb       : 16;
} __attribute__((packed)) tss;

void tss_initialize(void) {
  gdt_set_entry((uintptr_t)&tss, sizeof(tss), 0x89);
  tss_load();
}
