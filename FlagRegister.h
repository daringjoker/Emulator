//
// Created by DaringJoker on 9/15/2020.
//

#ifndef EMULATOR_FLAGREGISTER_H
#define EMULATOR_FLAGREGISTER_H
#include "constants.h"
typedef struct{
    unsigned int signFlag:1;
    unsigned int zeroFlag:1;
    unsigned int nc1;
    unsigned int auxCarryFlag:1;
    unsigned int nc2;
    unsigned int pairityFlag:1;
    unsigned int nc3;
    unsigned int carryFlag:1;
} FlagBits;
typedef union {
    Byte value;
    FlagBits bits;
} FlagContent;
class FlagRegister {
    FlagContent content;
};


#endif //EMULATOR_FLAGREGISTER_H
