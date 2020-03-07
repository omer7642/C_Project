.entry LIST
.extern fn1
    ;start of the program
                ;another line of test
MAIN:   add r0,LIST
        jsr fn1
LOOP:   prn #48
        lea     STR,r6
        inc r6
        mov *r6,L3
        sub r1,r4
        cmp r3,#-6
        bne END
        add r7,*r6 
        clr K
        sub L3,L3
.entry MAIN
        jmp LOOP
END:    stop
STR:    .string "abcd"
LIST:   .data   6,-9
        .data -100
K:      .data 31
;unimportant comment
.extern L3
;test line at the END