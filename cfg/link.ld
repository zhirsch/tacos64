ENTRY(start32)
SECTIONS
{
	HIDDEN(KERNEL_LMA = 0x0000000000100000);
	HIDDEN(KERNEL_VMA = 0xffffffff80000000);

	. = KERNEL_LMA;

	.rodata_nopg ALIGN(4K) :
	{
                KEEP(bld/multiboot_header.o(.rodata))
		KEEP(bld/start32.o(.rodata))
	}

	.data_nopg ALIGN(4K) :
	{
		KEEP(bld/start32.o(.data))
	}

	.bss_nopg ALIGN(4K) :
	{
		KEEP(bld/start32.o(.bss))
	}

	.text_nopg ALIGN(4K) :
	{
		KEEP(bld/start32.o(.text))
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
