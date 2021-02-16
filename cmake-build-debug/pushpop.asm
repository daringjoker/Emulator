lxi h,1234
mvi c,5
loop: push h
inx h
dcr c
jnz loop
nop 
mvi c,5
loop2: pop h
dcr c
jnz loop2
nop 
hlt