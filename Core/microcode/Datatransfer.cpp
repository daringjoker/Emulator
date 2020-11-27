//
// Created by DaringJoker on 11/10/2020.
//
#include "../Emulator.h"

using namespace std;

void Emulator::MOV(Decoded_Instr instr) {
    //MOV reg,reg
    char mnemonic[50], arg1[20], arg2[20];
    sscanf(instr.stencil.c_str(), "%s %s %s", mnemonic, arg1, arg2);
    arg1[1] = 0;
    if (string(arg1) != string("M") && string(arg2) != string("M")) {
        Byte temp = getRegByName(arg2).get();
        getRegByName(arg1).set(temp);
    } else {
        Word pointer = HL.get();
        if (arg1[0] == 'M') {
            Byte temp = getRegByName(arg2).get();
            Abus.Write(pointer);
            Dbus.write(temp);
            Ram.WriteMemory();
        } else {
            Abus.Write(pointer);
            Ram.ReadMemory();
            Byte temp = Dbus.read();
            getRegByName(arg1).set(temp);
        }

    }

}


void Emulator::MVI(Decoded_Instr instr) {
    //MVI reg,imm
    char mnemonic[50], arg1[20], arg2[20];
    sscanf(instr.stencil.c_str(), "%s %s %s", mnemonic, arg1, arg2);
    arg1[1] = 0;
    if (string(arg1) != string("M")) {
        Byte temp = instr.dataByte;
        getRegByName(arg1).set(temp);
    } else {
        Word pointer = HL.get();
        Byte temp = instr.dataByte;
        Abus.Write(pointer);
        Dbus.write(temp);
        Ram.WriteMemory();

    }

}

void Emulator::LXI(Decoded_Instr instr) {
    char mnemonic[50], arg1[20], arg2[20];
    sscanf(instr.stencil.c_str(), "%s %s %s", mnemonic, arg1, arg2);
    arg1[1] = 0;
    Word temp = instr.dataWord;
    getRegPairByName(arg1).set(temp);
}

void Emulator::LDA(Decoded_Instr instr) {
    Abus.Write(instr.dataWord);
    Ram.ReadMemory();
    Byte temp = Dbus.read();
    this->A.set(temp);
}

void Emulator::STA(Decoded_Instr instr) {
    Byte temp = this->A.get();
    Abus.Write(instr.dataWord);
    Dbus.write(temp);
    Ram.WriteMemory();
}

void Emulator::LHLD(Decoded_Instr instr) {
    Abus.Write(instr.dataWord);
    Ram.ReadMemory();
    Byte temp = Dbus.read();
    this->L.set(temp);

    Abus.Write(instr.dataWord + 1);
    Ram.ReadMemory();
    temp = Dbus.read();
    this->H.set(temp);
}

void Emulator::SHLD(Decoded_Instr instr) {
    Byte temp = this->L.get();
    Abus.Write(instr.dataWord);
    Dbus.write(temp);
    Ram.WriteMemory();

    temp = this->H.get();
    Abus.Write(instr.dataWord + 1);
    Dbus.write(temp);
    Ram.WriteMemory();
}

void Emulator::LDAX(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Word address = getRegPairByName(arg1).get();
    Abus.Write(address);
    Ram.ReadMemory();
    Byte temp = Dbus.read();
    this->A.set(temp);

}

void Emulator::STAX(Decoded_Instr instr) {
    char mnemonic[50], arg1[20];
    sscanf(instr.stencil.c_str(), "%s %s", mnemonic, arg1);
    Word address = getRegPairByName(arg1).get();
    Byte temp = this->A.get();
    Abus.Write(address);
    Dbus.write(temp);
    Ram.WriteMemory();
}

void Emulator::XCHG(Decoded_Instr instr) {
    Word temp = HL.get();
    Word temp2 = DE.get();
    HL.set(temp2);
    DE.set(temp);
}

