;file test2.as
Entries: .data 56 , 4 , 78,21,55,Hi
	.extern   fn1
MAIN:    bdd  	r3 , LIST
jsr   fn1
lea STR  , GS
inc   r6
mov  *r6  , L3
END:  		stop
STR:		.string  "abcd"
LIST:		.data   6 ,  -3009
	.data   -100
K:		.data  31
	.extern L3
sub  r1 , r4
cmp r3, r4
bne END
add  r7,  *r6
clr #88
sub L3 ,  S2
	.entry MAIN
jmp  sss

;end of file
