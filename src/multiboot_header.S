	#include <asm/multiboot/multiboot2.h>

	.section .rodata
	
	.macro chksum arch, size
	.long -(MULTIBOOT2_HEADER_MAGIC + \arch + \size)
	.endm

	.align	MULTIBOOT_HEADER_ALIGN
.L1:	
	.long	MULTIBOOT2_HEADER_MAGIC
	.long	MULTIBOOT_ARCHITECTURE_I386
	.long	.L2 - .L1
	chksum	MULTIBOOT_ARCHITECTURE_I386, (.L2 - .L1)

	.short	MULTIBOOT_HEADER_TAG_END
	.short	0  // not optional
	.long	8  // fixed size of 8 bytes
	
.L2:
