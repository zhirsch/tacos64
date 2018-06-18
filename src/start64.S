        .text
        .global start64
start64:
        // Set the stack.
        mov     $kernel_stack_start, %rsp

        // Reset the eflags.
        push    $0
        popf

	// Jump to the C entry point.
	call	kernel_main
        cli
        hlt
