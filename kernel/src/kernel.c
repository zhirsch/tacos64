#include <stdint.h>

#include "cpu/apic.h"
#include "cpu/idt.h"
#include "cpu/tss.h"
#include "multiboot2/multiboot2.h"
#include "string/snprintf.h"
#include "terminal/terminal.h"

struct rsdp_t {
  char Signature[8];
  uint8_t Checksum;
  char OEMID[6];
  uint8_t Revision;
  uint32_t RsdtAddress;
} __attribute__((packed));

struct rsdt_header_t {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};

static void parse_mbi_tags(uintptr_t mbi_addr);
static void parse_mbi_tag(struct multiboot_tag* tag);
static struct multiboot_tag* next_mbi_tag(struct multiboot_tag* tag);
static void parse_acpi1_rsdp_table(const struct rsdp_t* rsdp);

void kernel_main(unsigned int mbi_magic, unsigned int mbi_addr) {
  clear_screen();
  kprintf("Welcome to TacOS64!\n");
  kprintf("Multiboot2 magic=0x%x info=0x%x\n", mbi_magic, mbi_addr);

  kprintf("Parsing the mbi tags\n");
  parse_mbi_tags(mbi_addr);

  kprintf("Initializing the tss\n");
  tss_initialize();

  kprintf("Initializing the apic\n");
  apic_initialize();

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
      kprintf("ACPI (old) size: 0x%x\n", acpi->size);
      parse_acpi1_rsdp_table((struct rsdp_t*)acpi->rsdp);
      break;
    }
    case MULTIBOOT_TAG_TYPE_ACPI_NEW: {
      struct multiboot_tag_new_acpi* acpi = (struct multiboot_tag_new_acpi*)tag;
      kprintf("ACPI (new) size: 0x%x\n", acpi->size);
      break;
    }
  }
}

static struct multiboot_tag* next_mbi_tag(struct multiboot_tag* tag) {
  return (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
}

static void parse_acpi1_rsdp_table(const struct rsdp_t* rsdp) {
  kprintf("RSDP signature: %.8s\n", rsdp->Signature);
  kprintf("RSDP checksum: %x\n", rsdp->Checksum);
  kprintf("RSDP OEMID: %.6s\n", rsdp->OEMID);
  kprintf("RSDP revision: %x\n", rsdp->Revision);
  kprintf("RSDP rsdt address: %x\n", rsdp->RsdtAddress);

  struct rsdt_header_t* rsdt_header = (struct rsdt_header_t*)(uintptr_t)rsdp->RsdtAddress;
  kprintf("RSDT signature: %.4s\n", rsdt_header->Signature);
}