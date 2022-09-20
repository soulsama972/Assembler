.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
lea STR, r6
sub r1, r4
bne RR
cmp val1, #-6
bne END[r8]
dec K
dec #1
aaa: bbb, ccc
.entry MAIN
sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, --9
.data -100
.entry K
K: .data 31
.extern val1
