#pragma once

static inline void cpu_read_msr(uint32_t msr, uint32_t *lo, uint32_t *hi) {
  __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static void cpu_write_msr(uint32_t msr, uint32_t lo, uint32_t hi) {
  __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
