//
// Created by DaringJoker on 11/10/2020.
//

#include "../Emulator.h"

void Emulator::PCHL(Decoded_Instr instr) {
    Word temp = HL.get();
    PC.set(temp);
}

void Emulator::JMP(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    string mnemonics(mnemonic);
    if (meetsCondition(mnemonics)) {
        PC.set(instr.dataWord);
    }
}

bool Emulator::meetsCondition(const string &mnemonics) {
    string condition = mnemonics.substr(1, mnemonics.size() - 1);
    if (condition == "NZ" && flags.isSet_ZeroFlag())return false;
    if (condition == "Z" && !flags.isSet_ZeroFlag())return false;
    if (condition == "NC" && flags.isSet_CarryFlag()) return false;
    if (condition == "C" && !flags.isSet_CarryFlag())return false;
    if (condition == "PO" && flags.isSet_PairityFlag())return false;
    if (condition == "PE" && !flags.isSet_PairityFlag())return false;
    if (condition == "P" && flags.isSet_SignFlag())return false;
    if (condition == "M" && !flags.isSet_SignFlag()) return false;
    return true;
}

void Emulator::CALL(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    string mnemonics(mnemonic);
    if (meetsCondition(mnemonics)) {
        Word sp = SP.get();
        Word pch = PCh.get();
        Word pcl = PCl.get();

        Abus.Write(sp - 1);
        Dbus.write(pch);
        Ram.WriteMemory();

        Abus.Write(sp - 2);
        Dbus.write(pcl);
        Ram.WriteMemory();

        sp = sp - 2;
        SP.set(sp);

        PC.set(instr.dataWord);
    }
}

void Emulator::RET(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    string mnemonics(mnemonic);
    if (meetsCondition(mnemonics)) {
        if (meetsCondition(mnemonics)) {
            Word sp = SP.get();

            Abus.Write(sp);
            Ram.ReadMemory();
            Byte pcl = Dbus.read();
            PCl.set(pcl);

            Abus.Write(sp + 1);
            Ram.ReadMemory();
            Byte pch = Dbus.read();
            PCh.set(pch);

            sp = sp + 2;
            SP.set(sp);

        }
    }
}

void Emulator::RST(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
}