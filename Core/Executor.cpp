//
// Created by DaringJoker on 11/9/2020.
//
#include "emulator.h"
#include <iostream>
using namespace std;
void emulator::Execute(Decoded_Instr instr)
{
    cout<<instr.stencil<<endl;
    if (instr.stencil[0]=='M')MOV(instr);
    if(instr.stencil[0]=='H')halted=true;
}