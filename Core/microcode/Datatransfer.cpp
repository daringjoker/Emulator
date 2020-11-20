//
// Created by DaringJoker on 11/10/2020.
//
#include "../emulator.h"
using namespace std;
void emulator::MOV(Decoded_Instr instr) {
    //MOV reg,reg
    char mnemonic[50],arg1[20],arg2[20];
    sscanf(instr.stencil.c_str(),"%s %s %s",mnemonic,arg1, arg2);
    arg1[1]=0;
    if(string(arg1)!=string("M")&&string(arg2)!=string("M"))
    {
        Byte temp=getRegByName(arg2).get();
        getRegByName(arg1).set(temp);
    }
    else
    {
        Word pointer=HL.get();
        if(arg1[0]=='M'){
            Byte temp=getRegByName(arg2).get();
            Abus.Write(pointer);
            Dbus.write(temp);
            Ram.WriteMemory();
        }
        else{
            Abus.Write(pointer);
            Ram.ReadMemory();
            Byte temp=Dbus.read();
            getRegByName(arg1).set(temp);
        }

    }

}


void emulator::MVI(Decoded_Instr instr) {
  //MVI reg,imm
    char mnemonic[50],arg1[20],arg2[20];
    sscanf(instr.stencil.c_str(),"%s %s %s",mnemonic,arg1, arg2);
    arg1[1]=0;
    if(string(arg1)!=string("M"))
    {
        Byte temp=instr.dataByte;
        getRegByName(arg1).set(temp);
    }
    else
    {
        Word pointer=HL.get();
        Byte temp=instr.dataByte;
        Abus.Write(pointer);
        Dbus.write(temp);
        Ram.WriteMemory();

    }

}