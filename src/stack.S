        .bss
        .align  0x1000
kernel_stack_end:
        .skip   0x4000
        .global kernel_stack_start
kernel_stack_start:

