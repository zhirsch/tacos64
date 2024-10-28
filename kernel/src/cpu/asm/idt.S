        .text
        .global idt_load
        .type   idt_load, @function
        .size   idt_load, .idt_load_end - idt_load
idt_load:
        lidt    (idtp)
        ret
.idt_load_end:


        .data
        .global idt
        .align  0x10
idt:
        .skip   256 * 16, 0

idtp:
        .word   idtp - idt - 1
        .quad   idt
