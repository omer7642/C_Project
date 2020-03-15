;file test2.as
Entries: .data 56 , 4 , 78,21,55,90
	.extern   fn1
MAIN:    bdd  	r3 , LIST
jsr   fn1
lea STR  , r6
inc   r6
mov  *r6  , L3
sub  r1 , r4
cmp r3,  #-66,
bne END
add  r7,  *r6
clr r7 , r2
sub L3 ,  L3
	.entry MAIN
jmp  LOOP
END:  		stop
STR:		.string  "abcd"
LIST:		.data   6 ,  -3009
	.data   -100
K:		.data  31
	.extern L3
;end of file
