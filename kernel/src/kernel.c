#include <stddef.h>
#include <stdint.h>

#include "cpu/apic.h"
#include "cpu/idt.h"
#include "cpu/tss.h"
#include "multiboot/multiboot2.h"
#include "string/snprintf.h"
#include "string/strncmp.h"
#include "terminal/terminal.h"

struct rsdp_t {
  char Signature[8];
  uint8_t Checksum;
  char OEMID[6];
  uint8_t Revision;
  uint32_t RsdtAddress;
} __attribute__((packed));

struct sdt_header_t {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__((packed));

struct sdt_t {
  struct sdt_header_t header;
  uint32_t sdts[0];
} __attribute__((packed));

struct madt_t {
  struct sdt_header_t header;
  uint32_t local_apic_addr;
  uint32_t flags;
} __attribute__((packed));

struct madt_entry_t {
  uint8_t type;
  uint8_t length;
} __attribute__((packed));

struct madt_local_apic_entry_t {
  struct madt_entry_t header;
  uint8_t acpi_processor_id;
  uint8_t apic_id;
  uint32_t flags;
} __attribute__((packed));

struct madt_io_apic_entry_t {
  struct madt_entry_t header;
  uint8_t apic_id;
  uint8_t reserved;
  uint32_t apic_address;
  uint32_t global_system_interrupt_base;
} __attribute__((packed));

struct madt_io_apic_interrupt_source_override_entry_t {
  struct madt_entry_t header;
  uint8_t bus_source;
  uint8_t irq_source;
  uint32_t global_system_interrupt;
  uint16_t flags;
} __attribute__((packed));

static void parse_mbi_tags(uintptr_t mbi_addr);
static void parse_mbi_tag(struct multiboot_tag* tag);
static struct multiboot_tag* next_mbi_tag(struct multiboot_tag* tag);
static void parse_rsdp(const struct rsdp_t* rsdp);
static void parse_madt(struct madt_t* madt);
static void parse_madt_entry(struct madt_entry_t* entry);

static uintptr_t local_apic_base = 0;

void kernel_main(unsigned int mbi_magic, unsigned int mbi_addr) {
  clear_screen();
  kprintf("Welcome to TacOS64!\n");
  kprintf("Multiboot2 magic=0x%x info=0x%x\n", mbi_magic, mbi_addr);

  kprintf("Parsing the mbi tags\n");
  parse_mbi_tags(mbi_addr);

  kprintf("Initializing the tss\n");
  tss_initialize();

  kprintf("Initializing the apic\n");
  apic_initialize(local_apic_base);

  kprintf("Initializing the idt\n");
  idt_initialize();

  kprintf("Enabling interrupts\n");
  __asm__("sti");
}

static void parse_mbi_tags(uintptr_t addr) {
  struct multiboot_tag* tag = (struct multiboot_tag*)(addr + 8);
  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    parse_mbi_tag(tag);
    tag = next_mbi_tag(tag);
  }
  tag = next_mbi_tag(tag);
}

static void parse_mbi_tag(struct multiboot_tag* tag) {
  switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_ACPI_OLD: {
      struct multiboot_tag_old_acpi* acpi = (struct multiboot_tag_old_acpi*)tag;
      parse_rsdp((struct rsdp_t*)acpi->rsdp);
      break;
    }
  }
}

static struct multiboot_tag* next_mbi_tag(struct multiboot_tag* tag) {
  return (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
}

static void parse_rsdp(const struct rsdp_t* rsdp) {
  struct sdt_t* rsdt = (struct sdt_t*)(uintptr_t)rsdp->RsdtAddress;
  int entries = (rsdt->header.Length - sizeof(struct sdt_header_t)) / 4;
  for (int i = 0; i < entries; i++) {
    struct sdt_t* sdt = (struct sdt_t*)(uintptr_t)rsdt->sdts[i];
    if (strncmp(sdt->header.Signature, "APIC", 4) == 0) {
      parse_madt((struct madt_t*)sdt);
    }
  }
}

static void parse_madt(struct madt_t* madt) {
  kprintf("MADT local apic addr: 0x%08x\n", madt->local_apic_addr);
  local_apic_base = madt->local_apic_addr;
  uint32_t offset = sizeof(struct madt_t);
  while (offset < madt->header.Length) {
    struct madt_entry_t* entry = (struct madt_entry_t*)(((uint8_t*)madt) + offset);
    parse_madt_entry(entry);
    offset += entry->length;
  }
}

static void parse_madt_entry(struct madt_entry_t* entry) {
  switch (entry->type) {
    case 0: {
      // struct madt_local_apic_entry_t* e = (struct madt_local_apic_entry_t*)entry;
      break;
    }
    case 1: {
      // struct madt_io_apic_entry_t* e = (struct madt_io_apic_entry_t*)entry;
      break;
    }
    case 2: {
      // struct madt_io_apic_interrupt_source_override_entry_t* e =
      //     (struct madt_io_apic_interrupt_source_override_entry_t*)entry;
      break;
    }
    default:
      kprintf("Unknown MADT entry type (0x%x), ignoring\n", entry->type);
      break;
  }
}