.entry LOOP
.extern L01d!

MAIN:                                                                                                              add    r0 , l3

LOOP:    cmp r2, r3
    jsr LOOP
    jmp END
    cmp r9, x

END: stop
x:   .data 7,8,-9,
;comment in last line