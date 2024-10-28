        .text
        .global start64
        .type   start64, @function
        .size   start64, .start64_end - start64
start64:
        mov     $kernel_stack_start, %rsp
        call    kernel_main
        jmp     halt
.start64_end:
