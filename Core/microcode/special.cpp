//
// Created by DaringJoker on 11/21/2020.
//
#include "../Emulator.h"

void Emulator::NOP(Decoded_Instr instr) {
}

void Emulator::HLT(Decoded_Instr instr) {
    this->halted = true;
}