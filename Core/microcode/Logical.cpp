//
// Created by DaringJoker on 11/10/2020.
//

#include "../Emulator.h"

void Emulator::ANA(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a & temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ANI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a & temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::XRA(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a ^temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::XRI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a ^temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ORA(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a | temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ORI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a | temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::CMP(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a - temp;
    AdjustFlags(a, temp, result);
}

void Emulator::CPI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a -temp;
    AdjustFlags(a, temp, result);
}

void Emulator::RLC(Decoded_Instr instr) {
    Word temp = A.get();
    temp = temp << 1;
    Byte carry = (temp & 0x100) >> 9;
    temp |= carry;
    A.set(temp & 0xff);
    if (carry)
        flags.setCarryFlag();
    else
        flags.resetCarryFlag();

}

void Emulator::RRC(Decoded_Instr instr) {
    Word temp = A.get();
    Byte carry = (temp & 0x01);
    temp = temp >> 1;
    if (carry) {
        flags.setCarryFlag();
        temp |= 0b10000000;
    } else {
        flags.resetCarryFlag();
    }
    A.set(temp & 0xff);
}

void Emulator::RAL(Decoded_Instr instr) {
    Word temp = A.get();
    temp = temp << 1;
    Byte to_carry = (temp & 0x100);
    temp |= (flags.isSet_CarryFlag() ? 1 : 0);
    A.set(temp & 0xff);
    if (to_carry)
        flags.setCarryFlag();
    else
        flags.resetCarryFlag();
}

void Emulator::RAR(Decoded_Instr instr) {
    Word temp = A.get();
    Byte to_carry = (temp & 0x01);
    temp = temp >> 1;
    temp |= (flags.isSet_CarryFlag() ? 0b10000000 : 0);
    A.set(temp & 0xff);
    if (to_carry)
        flags.setCarryFlag();
    else
        flags.resetCarryFlag();
}

void Emulator::CMA(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = (~a & 0xff);
    A.set(result & 0xff);
}

void Emulator::CMC(Decoded_Instr instr) {
    if (flags.isSet_CarryFlag())
        flags.resetCarryFlag();
    else
        flags.setCarryFlag();
}

void Emulator::STC(Decoded_Instr instr) {
    flags.setCarryFlag();
}



