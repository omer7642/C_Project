.entry LOOP
.extern L01d
MAIN: add r0,r3
LOOP:    cmp r2, #500
    dec L01d
    jsr LOOP
    jmp END
    cmp r5, x
.entry MAIN
END: stop
x:   .data 7,8,-300
;comment in last line