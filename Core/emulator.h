//
// Created by DaringJoker on 11/7/2020.
//

#ifndef EMULATOR_EMULATOR_H
#define EMULATOR_EMULATOR_H
#include "../Components/AllComponents.h"
using namespace std;


typedef struct Decoded_Instr {
    Word startAddress;
    Byte size=0;
    string stencil;
    Byte dataByte=0;
    Word dataWord=0;
    Byte rawinstr[3];
} Decoded_Instr;

class emulator {
public:
    Register A,B,C,D,E,H,L,PCl,PCh,SPl,SPh;
    FlagRegister flags;
    RegisterPair PSW=RegisterPair(A,flags);
    RegisterPair BC=RegisterPair(B,C);
    RegisterPair DE=RegisterPair(D,E);
    RegisterPair HL=RegisterPair(H,L);
    RegisterPair SP=RegisterPair(SPh,SPl);
    RegisterPair PC=RegisterPair(PCh,PCl);
    DataBus Dbus;
    AddressBus Abus;
    Memory Ram;
    Ports ports;
    Word start_of_code=0x4000;
public://methods
    //public methods for client use
    void load_binary();
    void single_step();
    void run();
    void Reset();
private:
    bool fatal_error;
private:
    //Errors during execution
    void record_error(string errorstring,Word address);
    //fetching api
    Byte fetchByte();
    Word fetchWord();
    //for decoding the instructions
    Decoded_Instr Decode(Byte Opcode);

};
#endif //EMULATOR_EMULATOR_H
