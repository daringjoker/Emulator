//
// Created by DaringJoker on 11/7/2020.
//

#include "Emulator.h"

void Emulator::load_binary(vector<Byte> &binary) {
    Word index = this->start_of_code;
    codeEnd = this->start_of_code + binary.size();
    for (auto opcode:binary) {
        Ram.mem[index++] = opcode;
    }
}

void Emulator::single_step() {
    Byte opcode = fetchByte();
    Decoded_Instr instr = Decode(opcode);
    Execute(instr);
}

void Emulator::run() {
    if(!halted) {
        do {
            single_step();
            brkhit = breakpoints.count(this->PC.get()) != 0;
        } while (!this->halted && !this->brkhit);
    }
}


void Emulator::reset() {
    this->PC.set(this->start_of_code);
    this->PSW.set(0);
    this->BC.set(0);
    this->DE.set(0);
    this->HL.set(0);
    this->SP.set(0xffff);
    this->halted=false;
    this->brkhit=false;
}

Register &Emulator::getRegByName(string regname) {
    if (regname == string("A")) return A;
    else if (regname == string("B")) return B;
    else if (regname == string("C")) return C;
    else if (regname == string("D")) return D;
    else if (regname == string("E")) return E;
    else if (regname == string("H")) return H;
    else if (regname == string("L")) return L;
    else if (regname == string("M")) return M;
    else
        record_error("Invalid register found ", 0, Errorclass::Warning);
}
RegisterPair& Emulator::getRegPairByName(const string& regname) {
    if (regname == string("PSW")) return PSW;
    else if (regname == string("B")) return BC;
    else if (regname == string("D")) return DE;
    else if (regname == string("H")) return HL;
    else if (regname == string("SP")) return SP;
    else
        record_error("Invalid registerpair found ", 0, Errorclass::Warning);
}

void Emulator::addbreakpoint(Word brkpoint) {
    breakpoints.insert(brkpoint);
}

void Emulator::rembreakpoint(Word brkpoint) {
    breakpoints.erase(brkpoint);
}
