        .text
        .global start64
        .type   start64, @function
start64:
        // Set the stack.
        mov     $kernel_stack_start, %rsp

        // Reset the eflags.
        push    $0
        popf

        // Jump to the C entry point, passing the multiboot info.
        mov	%rax, %rdi
        mov	%rbx, %rsi
        call	kernel_main
        cli
        hlt

        .size start64, . - start64
