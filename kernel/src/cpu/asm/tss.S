        .text
        .global tss_load
        .type   tss_load, @function
        .size tss_load, .tss_load_end - tss_load
tss_load:
        mov     $0x28, %ax
        ltr     %ax
        ret
.tss_load_end:
