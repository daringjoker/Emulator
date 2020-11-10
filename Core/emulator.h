//
// Created by DaringJoker on 11/7/2020.
//

#ifndef EMULATOR_EMULATOR_H
#define EMULATOR_EMULATOR_H
#include <vector>
#include "../Components/AllComponents.h"
#include "DataTypes.h"
using namespace std;

class emulator {
public:
    Register A=Register("A"),B=Register("B"),C=Register("C"),\
    D=Register("D"),E=Register("E"),H=Register("H"),L=Register("L")\
    ,PCl=Register("PC_low"),PCh=Register("PC_high"),SPl=Register("SP_low"),SPh=Register("SP_high");
    FlagRegister flags;
    RegisterPair PSW=RegisterPair(A,flags);
    RegisterPair BC=RegisterPair(B,C);
    RegisterPair DE=RegisterPair(D,E);
    RegisterPair HL=RegisterPair(H,L);
    RegisterPair SP=RegisterPair(SPh,SPl);
    RegisterPair PC=RegisterPair(PCh,PCl);
    DataBus Dbus;
    AddressBus Abus;
    Memory Ram=Memory(Abus,Dbus);
    Ports ports=Ports(Abus,Dbus);
    Word start_of_code=0x4000;
    vector<emu_error> Error_list;
public://methods
    //public methods for client use
    emulator()=default;
    void load_binary(vector<Byte> binary);
    void single_step();
    void run();
    void reset();
private:
    bool fatal_error=false;
    bool halted=false;
private:
    //Errors during execution
    void record_error(string errorstring,Word address,Errorclass errclass);
    //fetching api
    Byte fetchByte();
    Word fetchWord();
    //for decoding the instructions
    Decoded_Instr Decode(Byte Opcode);
    //Executing instructions
    void Execute(Decoded_Instr instr);

};
#endif //EMULATOR_EMULATOR_H
