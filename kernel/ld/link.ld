KERNEL_LMA = 0x0000000000100000;
KERNEL_VMA = 0xffffffff80000000;

ENTRY(start32)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
	. = KERNEL_LMA;

        .text.init ALIGN(4K) :
        {
        	KEEP(*(.multiboot2))
                *(.text.init)
        }

        .rodata.init ALIGN(4K) (READONLY) :
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
		*(.text*)
	}

	.rodata ALIGN(4K) (READONLY) : AT(ADDR(.rodata) - KERNEL_VMA)
	{
		*(.rodata*)
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
	{
		*(.data*)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
	{
		*(.bss*)
		*(COMMON)
	}

        /DISCARD/ :
        {
                KEEP(*(.eh_frame))
        }
}
