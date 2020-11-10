//
// Created by DaringJoker on 11/7/2020.
//

#include "emulator.h"

void emulator::load_binary(vector<Byte> binary) {
    Word index=this->start_of_code;
    for(auto opcode:binary){
        Ram.mem[index++]=opcode;
    }
}

void emulator::single_step() {
    Byte opcode=fetchByte();
    Decoded_Instr instr=Decode(opcode);
    Execute(instr);
}

void emulator::run() {
        while(!this->halted)
        {
            single_step();
        }
}

void emulator::reset() {
    this->PC.set(this->start_of_code);
    this->PSW.set(0);
    this->BC.set(0);
    this->DE.set(0);
    this->HL.set(0);
    this->SP.set(0xffff);
}
