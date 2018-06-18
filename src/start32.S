	.section	.text
	.code32

	.macro set_page_entry addr, flags, table, offset
	mov	\addr, %eax
	or	\flags, %eax
	mov	\table, %edi
	mov	%eax, (\offset * 8)(%edi)
	.endm

	.macro set_page_contg base, flags, table
	mov	\base, %ebx
	or	\flags, %ebx
	mov	\table, %edi
	mov	$512, %ecx
1:	mov	%ebx, (%edi)
	add	$0x1000, %ebx
	add	$8, %edi
	loop 1b
	.endm

	.global	start32
start32:

	// Initialize paging.

	// Identity map the first 2 MiB:
	//
	// PML4T[0]->PDPT[0]->PD[0]->PT (first 2 MiB of physical)
	set_page_entry $.Lpdpt, $0x3, $kernel_pml4t, 0
	set_page_entry $.Lpd, $0x3, $.Lpdpt, 0
	set_page_entry $.Lpt, $0x3, $.Lpd, 0
	set_page_contg $0x00000000, $0x3, $.Lpt

	// Map the first 2 MiB of the kernel's virtual address space:
	//
	// PML4T[511]->PDPT[510]->PD[0]->PT (first 2 MiB of physical).
	set_page_entry $.Lpdpt_2, $0x3, $kernel_pml4t, 511
	set_page_entry $.Lpd_2, $0x3, $.Lpdpt_2, 510
	set_page_entry $.Lpt_2, $0x3, $.Lpd_2, 0
	set_page_contg $0x00000000, $0x3, $.Lpt_2

	// Set the base paging register.
	mov	$kernel_pml4t, %eax
	mov	%eax, %cr3

	// Enable PAE paging
	mov 	%cr4, %eax
	or	$1 << 5, %eax
	mov	%eax, %cr4

	// Enable long mode.
	mov	$0xC0000080, %ecx
	rdmsr
	or	$1 << 8, %eax
	wrmsr

	// Enable paging.
	mov	%cr0, %eax
	or	$1 << 31, %eax
	mov	%eax, %cr0

	// Set the 64-bit GDT (leave 32-bit compat mode).
	lgdt	(.Lgdtp)
	ljmpl	$0x8, $1f
1:      jmp     start64

	.section .bss
	// Define the PML4T as all zeros, i.e. nothing mapped.
kernel_pml4t:
	.skip	512 * 8

	// Make room for the initial identity mapping of the first 2 MiB.
.Lpdpt:
	.skip	512 * 8
.Lpd:
	.skip	512 * 8
.Lpt:
	.skip	512 * 8
.Lpdpt_2:
	.skip	512 * 8
.Lpd_2:
	.skip	512 * 8
.Lpt_2:
	.skip	512 * 8


	.section .rodata
.Lgdt:
	// null
	.word	0xffff
	.word	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0

	// code
	.word	0
	.word	0
	.byte	0
	.byte	0x9a
	.byte	0xaf
	.byte	0

	// data
	.word	0
	.word	0
	.byte	0
	.byte	0x82
	.byte	0
	.byte	0

.Lgdtp:
	.word	.Lgdtp - .Lgdt - 1
	.long	.Lgdt
