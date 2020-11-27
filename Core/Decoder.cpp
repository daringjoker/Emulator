//
// Created by DaringJoker on 11/9/2020.
//

#ifndef EMULATOR_DECODER_CPP
#define EMULATOR_DECODER_CPP

#include "Emulator.h"
#include "../InstructionSet/Instructions.h"
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

Decoded_Instr Emulator::Decode(Byte opcode) {
    Instruction instr;
    try {
        instr = opcode2Instr.at(opcode);
    }
    catch (out_of_range &oor) {
        char asString[5];
        sprintf(asString, "0x%02X", opcode);
        record_error(string("Invalid instruction opcode :") + string(asString), PC.get() - 1, Errorclass::Fatal_error);
        fatal_error = true;
        return {};
    }
    Decoded_Instr decoded;
    decoded.startAddress = this->PC.get() - 1;
    decoded.rawinstr[0] = opcode;
    decoded.size = instr.Instruction_length;
    decoded.stencil = instr.Pattern;
    if (decoded.size == 2) {
        decoded.dataByte = fetchByte();
        decoded.rawinstr[1] = decoded.dataByte;
    } else if (decoded.size == 3) {
        decoded.dataWord = fetchWord();
        decoded.rawinstr[1] = decoded.dataWord & 0xff;
        decoded.rawinstr[2] = (decoded.dataWord >> 8) & 0xff;
    }
    return decoded;
}


#endif //EMULATOR_DECODER_CPP
