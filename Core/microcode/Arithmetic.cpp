//
// Created by DaringJoker on 11/10/2020.
//
#include "../Emulator.h"

#define signbitof(reg) (reg&0b10000000)>>8

bool isoddPairity(Byte n) {
    Byte y = n ^(n >> 1);
    y = y ^ (y >> 2);
    y = y ^ (y >> 4);
    y = y ^ (y >> 8);
    return y == 1;
}

void Emulator::AdjustFlags(Byte operand1, Byte operand2, int result) {
    if (result == 0)flags.setZeroFlag();
    else flags.resetZeroFlag();
    if (result > 0xff || result < 0)flags.setCarryFlag();
    else flags.resetCarryFlag();
    if (signbitof(result) == 1)flags.setSignFlag();
    else flags.resetSignFlag();
    if (isoddPairity(result))flags.setPairityFlag();
    else flags.resetPairityFlag();
    int ac = (operand1 & 0xf) + (operand2 & 0xf);
    if (ac > 0xf || ac < 0)flags.resetAuxCarryFlag();
    else flags.setAuxCarryFlag();
}

void Emulator::DCR(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte temp = getRegByName(arg1).get();
    int result = temp - 1;
    getRegByName(arg1).set(result & 0xff);
    AdjustFlags(temp + 1, 1, result);
}

void Emulator::INR(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte temp = getRegByName(arg1).get();
    int result = temp + 1;
    getRegByName(arg1).set(result & 0xff);
    AdjustFlags(temp - 1, 1, result);
}

void Emulator::ADD(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a + temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ADI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a + temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ADC(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a + temp;
    if (flags.isSet_CarryFlag())result += 1;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::ACI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a + temp;
    if (flags.isSet_CarryFlag())result += 1;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::SUB(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a - temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::SUI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a - temp;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::SBB(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Byte a = A.get();
    Byte temp = getRegByName(arg1).get();
    int result = a - temp;
    if (flags.isSet_CarryFlag())result -= 1;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);

}

void Emulator::SBI(Decoded_Instr instr) {
    Byte a = A.get();
    Byte temp = instr.dataByte;
    int result = a - temp;
    if (flags.isSet_CarryFlag())result -= 1;
    A.set(result & 0xff);
    AdjustFlags(a, temp, result);
}

void Emulator::INX(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    int temp = getRegPairByName(arg1).get();
    temp = temp + 1;
    getRegPairByName(arg1).set(temp&0xffff);
    if(temp>0xffff)this->flags.setCarryFlag();
    else this->flags.resetCarryFlag();

}

void Emulator::DCX(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    int temp = getRegPairByName(arg1).get();
    temp = temp - 1;
    getRegPairByName(arg1).set(temp&0xffff);
    if(temp<0)this->flags.setCarryFlag();
    else this->flags.resetCarryFlag();
}

void Emulator::DAD(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Word temp = getRegPairByName(arg1).get();
    Word hl = HL.get();
    int result = hl + temp;
    HL.set(result & 0xffff);
    if (result > 0xffff)flags.setCarryFlag();
    else flags.resetCarryFlag();
}

void Emulator::DAA(Decoded_Instr instr) {
    Byte acc = A.get();
    Byte LowNibble = acc & 0xf;
    Byte HighNibble = acc & 0xf0;
    Byte toBeAdded = 0;
    if (LowNibble > 9 || flags.isSet_AuxCarryFlag()) {
        toBeAdded += 6;
        HighNibble++;
    }
    if (HighNibble > 9 || flags.isSet_CarryFlag()) {
        toBeAdded += 0x60;
    }
    int result = acc + toBeAdded;
    A.set(result & 0xff);
    AdjustFlags(acc, toBeAdded, result);
}