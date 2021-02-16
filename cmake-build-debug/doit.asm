;8085 Program to sort an
;array in ascending order
		 MVI A,3h
		 STA 5000h
		 MVI A,60h
		 STA 5001h
		 MVI A,55h
		 STA 5002h
		 MVI A,50h
		 STA 5003h
         LXI H, 5000h   ;Starting address of array, stores array size
         MOV C, M     ;Store array size in C, used as Counter for OuterLoop
         DCR C         ;Decrement OutLoop counter

OutLoop: MOV D, C      ;Copy counter in D, used as InLoop counter

         LXI H, 5001h   ;5001 stores 1st element of array

InLoop:  MOV A, M      ;store element of array in A
         INX H         ;goto next address
         CMP M         ;compare A (element) with next element

         JC Skip       ;if A < M, jump to skip
         MOV B, M      ;Swap elements
         MOV M, A
         DCX H
         MOV M,B
         INX H
   Skip: DCR D
          nop
          nop;Decrement InLoop counter
         JNZ InLoop    ;if D!=0 jump to InLoop

         DCR C         ;Decrement OutLoop counter
         jnz OutLoop   ;if C!=0 jump to OutLoop
         HLT           ;HALT program
