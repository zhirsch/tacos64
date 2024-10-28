        .section .text.init, "ax"
        .code32

        .macro set_page_entry addr, flags, table, offset
        mov    \addr, %eax
        or     \flags, %eax
        mov    \table, %edi
        mov    %eax, (\offset * 8)(%edi)
        .endm

        .macro set_page_contg base, flags, table
        mov    \base, %ebx
        or     \flags, %ebx
        mov    \table, %edi
        mov    $512, %ecx
1:      mov    %ebx, (%edi)
        add    $0x1000, %ebx
        add    $8, %edi
        loop   1b
        .endm

        .global start32
        .type   start32, @function
start32:
        // Set the stack.
        mov     $kernel_stack_start, %esp

        // Reset the eflags.
        push    $0
        popf

        // Save values passed from the multiboot2 loader.
        push    %eax
        push    %ebx

        // Initialize paging.

        // Identity map the first 2 MiB:
        //
        // PML4T[0]->PDPT[0]->PD[0]->PT (first 2 MiB of physical)
        set_page_entry $.Lpdpt, $0x3, $kernel_pml4t, 0
        set_page_entry $.Lpd, $0x3, $.Lpdpt, 0
        set_page_entry $.Lpt, $0x3, $.Lpd, 0
        set_page_contg $0x00000000, $0x3, $.Lpt

        // HACK: map the ACPI tables
        set_page_entry $.Lpt_3, $0x3, $.Lpd, 15
        set_page_contg $0x01e00000, $0x1, $.Lpt_3

        // HACK: map the APIC registers
        set_page_entry $.Lpd_4, $0x3, $.Lpdpt, 3
        set_page_entry $.Lpt_4, $0x3, $.Lpd_4, 503
        set_page_contg $0xfee00000, $0x3, $.Lpt_4

        // Map the first 2 MiB of the kernel's virtual address space:
        //
        // PML4T[511]->PDPT[510]->PD[0]->PT (first 2 MiB of physical).
        set_page_entry $.Lpdpt_2, $0x3, $kernel_pml4t, 511
        set_page_entry $.Lpd_2, $0x3, $.Lpdpt_2, 510
        set_page_entry $.Lpt_2, $0x3, $.Lpd_2, 0
        set_page_contg $0x00000000, $0x3, $.Lpt_2

        // Set the base paging register.
        mov     $kernel_pml4t, %eax
        mov     %eax, %cr3

        // Enable PAE paging
        mov     %cr4, %eax
        or      $1 << 5, %eax
        mov     %eax, %cr4

        // Enable long mode.
        mov     $0xC0000080, %ecx
        rdmsr
        or      $1 << 8, %eax
        wrmsr

        // Enable paging.
        mov     %cr0, %eax
        or      $1 << 31, %eax
        mov     %eax, %cr0

        // Restore the multiboot2 values.
        pop     %esi
        pop     %edi

        // Set the 64-bit GDT (leave 32-bit compat mode).
        lgdt    (gdtp)
        ljmpl   $0x8, $1f
1:      mov     $0x10, %ax
        mov     %ax, %ds
        mov     %ax, %es
        mov     %ax, %fs
        mov     %ax, %gs
        mov     %ax, %ss

        jmp     start64

        .size start32, . - start32


        .section .bss.init, "aw", @nobits
        .align  0x1000
kernel_stack_end:
        .skip   0x4000
        .global kernel_stack_start
kernel_stack_start:

kernel_pml4t:
        .skip   512 * 8
.Lpdpt:
        .skip   512 * 8
.Lpd:
        .skip   512 * 8
.Lpt:
        .skip   512 * 8
.Lpdpt_2:
        .skip   512 * 8
.Lpd_2:
        .skip   512 * 8
.Lpt_2:
        .skip   512 * 8
.Lpt_3:
        .skip   512 * 8
.Lpd_4:
        .skip   512 * 8
.Lpt_4:
        .skip   512 * 8

// GDT
// byte | description
// -----+------------
//  0   | limit1
//  1   | limit2
//  2   | base1
//  3   | base2
//  4   | base3
//  5   | access
//  6   | hi: flags, lo: limit3
//  7   | base4

        .section .data.init, "aw", @progbits
        .global gdt
        .align  0x10
gdt:
        // null
        .skip   8, 0

        // kernel code
        .byte   0xff
        .byte   0xff
        .byte   0
        .byte   0
        .byte   0
        .byte   0x9b
        .byte   0xaf
        .byte   0

        // kernel data
        .byte   0xff
        .byte   0xff
        .byte   0
        .byte   0
        .byte   0
        .byte   0x93
        .byte   0xcf
        .byte   0

        // user code
        .byte   0xff
        .byte   0xff
        .byte   0
        .byte   0
        .byte   0
        .byte   0xfb
        .byte   0xaf
        .byte   0

        // user data
        .byte   0xff
        .byte   0xff
        .byte   0
        .byte   0
        .byte   0
        .byte   0xf3
        .byte   0xcf
        .byte   0

        // tss
        .skip   16, 0

gdtp:
        .word   gdtp - gdt - 1
        .quad   gdt