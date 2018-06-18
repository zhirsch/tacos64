ENTRY(_start)
SECTIONS
{
	HIDDEN(KERNEL_LMA = 0x0000000000100000);
	HIDDEN(KERNEL_VMA = 0xffffffff80000000);

	. = KERNEL_LMA;

	.rodata_multiboot_header ALIGN(4K) :
	{
		KEEP(bld/multiboot_header.o(.rodata))
	}

	.text_boot ALIGN(4K) :
	{
		KEEP(bld/boot.o(.text))
	}

	.data_boot ALIGN(4K) :
	{
		KEEP(bld/boot.o(.data))
	}

	.rodata_boot ALIGN(4K) :
	{
		KEEP(bld/boot.o(.rodata))
	}

	. += KERNEL_VMA;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
	{
		*(.rodata*)
	}

	.eh_frame ALIGN(4K) : AT(ADDR(.eh_frame) - KERNEL_VMA)
	{
		KEEP(*(.eh_frame))
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
	{
		*(.bss)
		*(COMMON)
	}

	/DISCARD/ :
	{
		*(.comment)
	}
}
