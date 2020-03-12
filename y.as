;start of the program
.extern fn1
.extern L3
        ;another line of test
MAIN:   add r0,LIST
        jsr fn1
LOOP:   prn #48
        lea  
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
STR:    .string "abcd
        .string         abcd"
LIST:   .data
        .string 
        .data -100
K:      .data 31
;unimportant comment

;test line at the END