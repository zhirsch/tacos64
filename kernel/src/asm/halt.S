        .text
        .global halt
        .type halt, @function
halt:
        cli
1:      hlt
        jmp 1b