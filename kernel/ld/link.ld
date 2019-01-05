KERNEL_LMA = 0x0000000000100000;
KERNEL_VMA = 0xffffffff80000000;

ENTRY(start32)
SECTIONS
{
	. = KERNEL_LMA;

        .rodata.multiboot2 ALIGN(4K) :
        {
                KEEP(*(.multiboot2))
        }

        .text.init ALIGN(4K) :
        {
                *(.text.init)
        }

        .rodata.init ALIGN(4K) :
        {
                *(.rodata.init)
        }

        .data.init ALIGN(4K) :
        {
                *(.data.init)
        }

        .bss.init ALIGN(4K) :
        {
                *(.bss.init)
        }

	. += KERNEL_VMA;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
	{
		*(.rodata)
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
                KEEP(*(.eh_frame))
        }
}
