//
// Created by DaringJoker on 11/9/2020.
//
#include "emulator.h"
#include <iostream>
using namespace std;
void emulator::Execute(Decoded_Instr instr)
{
    cout<<instr.stencil<<endl;
    char mnemonic[20],arg1[10],arg2[10];
    sscanf(instr.stencil.c_str(),"%s%s,%s",mnemonic,arg1,arg2);
    cout<<"mnemonic : "<<mnemonic<<"\n\targument1 : "<<arg1<<"\n\targument2 : "<<arg2<<endl;
    if(instr.stencil[0]=='H')halted=true;
}