#pragma once

#include <stdint.h>

extern void gdt_set_entry(uintptr_t base, uint32_t limit, uint8_t access);