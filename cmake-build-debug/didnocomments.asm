         MVI C, 00h    
         LDA 5000h     
         MOV B, A     
         DCR B        
         ORA A        
         JZ Exit      
         LDA 5001h     
         MOV D, A     
         ORA A        
   Loop: JZ Exit      
         ADD D        
         JNC NoCarry  
         INR C        
NoCarry: DCR B        
         JMP Loop     
   Exit: STA 5003h     
         MOV A, C     
         STA 5002h     
         HLT