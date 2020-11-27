//
// Created by DaringJoker on 11/9/2020.
//

#ifndef EMULATOR_DATATYPES_H
#define EMULATOR_DATATYPES_H

#include "../Components/AllComponents.h"

using namespace std;

//This struct represent an instruction after being decoded from the memory
typedef struct Decoded_Instr {
    Word startAddress;
    Byte size = 0;
    string stencil;
    Byte dataByte = 0;
    Word dataWord = 0;
    Byte rawinstr[3];
} Decoded_Instr;

enum Errorclass {
    Warning, Internal_error, Fatal_error, Debug_report
};
typedef struct emu_error {
    string Errorstring;
    Word Address;
    Errorclass Errclass;
} emu_error;
#endif //EMULATOR_DATATYPES_H
