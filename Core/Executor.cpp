//
// Created by DaringJoker on 11/9/2020.
//
#include "Emulator.h"
#include <iostream>

using namespace std;

void Emulator::Execute(Decoded_Instr instr) {
    int end = min(instr.stencil.find(' '), instr.stencil.length());
    string mnemonic = instr.stencil.substr(0, end);
    if (mnemonic == "ACI")ACI(instr);
    else if (mnemonic == "ADC")ADC(instr);
    else if (mnemonic == "ADD")ADD(instr);
    else if (mnemonic == "ADI")ADI(instr);
    else if (mnemonic == "ANA")ANA(instr);
    else if (mnemonic == "ANI")ANI(instr);
    else if (mnemonic == "CALL")CALL(instr);
    else if (mnemonic == "CC")CALL(instr);
    else if (mnemonic == "CM")CALL(instr);
    else if (mnemonic == "CMA")CMA(instr);
    else if (mnemonic == "CMC")CMC(instr);
    else if (mnemonic == "CMP")CMP(instr);
    else if (mnemonic == "CNC")CALL(instr);
    else if (mnemonic == "CNZ")CALL(instr);
    else if (mnemonic == "CP")CALL(instr);
    else if (mnemonic == "CPE")CALL(instr);
    else if (mnemonic == "CPI")CPI(instr);
    else if (mnemonic == "CPO")CALL(instr);
    else if (mnemonic == "CZ")CALL(instr);
    else if (mnemonic == "DAA")DAA(instr);
    else if (mnemonic == "DAD")DAD(instr);
    else if (mnemonic == "DCR")DCR(instr);
    else if (mnemonic == "DCX")DCX(instr);
    else if (mnemonic == "DI")DI(instr);
    else if (mnemonic == "EI")EI(instr);
    else if (mnemonic == "HLT")HLT(instr);
    else if (mnemonic == "IN")IN(instr);
    else if (mnemonic == "INR")INR(instr);
    else if (mnemonic == "INX")INX(instr);
    else if (mnemonic == "JC")JMP(instr);
    else if (mnemonic == "JM")JMP(instr);
    else if (mnemonic == "JMP")JMP(instr);
    else if (mnemonic == "JNC")JMP(instr);
    else if (mnemonic == "JNZ")JMP(instr);
    else if (mnemonic == "JP")JMP(instr);
    else if (mnemonic == "JPE")JMP(instr);
    else if (mnemonic == "JPO")JMP(instr);
    else if (mnemonic == "JZ")JMP(instr);
    else if (mnemonic == "LDA")LDA(instr);
    else if (mnemonic == "LDAX")LDAX(instr);
    else if (mnemonic == "LHLD")LHLD(instr);
    else if (mnemonic == "LXI")LXI(instr);
    else if (mnemonic == "MOV")MOV(instr);
    else if (mnemonic == "MVI")MVI(instr);
    else if (mnemonic == "NOP")NOP(instr);
    else if (mnemonic == "ORA")ORA(instr);
    else if (mnemonic == "ORI")ORI(instr);
    else if (mnemonic == "OUT")OUT(instr);
    else if (mnemonic == "PCHL")PCHL(instr);
    else if (mnemonic == "POP")POP(instr);
    else if (mnemonic == "PUSH")PUSH(instr);
    else if (mnemonic == "RAL")RAL(instr);
    else if (mnemonic == "RAR")RAR(instr);
    else if (mnemonic == "RC")RET(instr);
    else if (mnemonic == "RET")RET(instr);
    else if (mnemonic == "RIM")RIM(instr);
    else if (mnemonic == "RLC")RLC(instr);
    else if (mnemonic == "RM")RET(instr);
    else if (mnemonic == "RNC")RET(instr);
    else if (mnemonic == "RNZ")RET(instr);
    else if (mnemonic == "RP")RET(instr);
    else if (mnemonic == "RPE")RET(instr);
    else if (mnemonic == "RPO")RET(instr);
    else if (mnemonic == "RRC")RRC(instr);
    else if (mnemonic == "RST")RST(instr);
    else if (mnemonic == "RZ")RET(instr);
    else if (mnemonic == "SBB")SBB(instr);
    else if (mnemonic == "SBI")SBI(instr);
    else if (mnemonic == "SHLD")SHLD(instr);
    else if (mnemonic == "SIM")SIM(instr);
    else if (mnemonic == "SPHL")SPHL(instr);
    else if (mnemonic == "STA")STA(instr);
    else if (mnemonic == "STAX")STAX(instr);
    else if (mnemonic == "STC")STC(instr);
    else if (mnemonic == "SUB")SUB(instr);
    else if (mnemonic == "SUI")SUI(instr);
    else if (mnemonic == "XCHG")XCHG(instr);
    else if (mnemonic == "XRA")XRA(instr);
    else if (mnemonic == "XRI")XRI(instr);
    else if (mnemonic == "XTHL")XTHL(instr);
}