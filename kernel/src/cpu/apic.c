#include "cpu/apic.h"

#include <cpuid.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu/cpuid.h"
#include "cpu/msr.h"
#include "logging/panic.h"
#include "terminal/terminal.h"

#define IA32_APIC_BASE_MSR 0x1b
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

static bool is_apic_present(void) {
  unsigned int eax, ebx, ecx, edx;
  int result = __get_cpuid(1, &eax, &ebx, &ecx, &edx);
  return result == 1 && edx & CPUID_FEAT_EDX_APIC;
}

static void enable_local_apic(uintptr_t base) {
  uint32_t eax, edx;
  cpu_read_msr(IA32_APIC_BASE_MSR, &eax, &edx);
  if (eax & IA32_APIC_BASE_MSR_BSP) {
    kprintf("Running on the bootstrap processor, great!\n");
  } else {
    panic("%s", "Uh, oh... not running on the bootstrap processor...\n");
  }
  cpu_write_msr(IA32_APIC_BASE_MSR, (base & 0xfffff000) | IA32_APIC_BASE_MSR_ENABLE, 0);
}

void apic_initialize(uintptr_t base) {
  if (is_apic_present()) {
    enable_local_apic(base);
    kprintf("Local APIC is at 0x%lx\n", base);

    uint32_t* local_apic_id = (uint32_t*)(base + 0x20);
    kprintf("Local APIC id is %x\n", *local_apic_id);
  } else {
    panic("%s", "no APIC present");
  }
}