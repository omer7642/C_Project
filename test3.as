;file test2.as
Entries: .data 56 , 4 , 78,21,55,90
	.extern   fn1
    .extern L3
	.entry LOOP
MAIN:    add  	r3 , LIST
LOOP: prn #48
jsr   fn1
lea STR  , r6
inc   r6
mov  *r6  , L3
sub  r1 , r4
END:  		stop
STR:		.string  "abcd"
LIST:		.data   6 ,  -3009

cmp r3,  #-6
bne END
add  r7,  *r6
clr r7
sub L3 ,  L3
	.entry MAIN
	.data   -100
K:		.data  31
jmp  LOOP


;end of file