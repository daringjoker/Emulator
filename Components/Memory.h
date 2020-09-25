//
// Created by DaringJoker on 9/23/2020.
//

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H
#include "AddressBus.h"
#include "DataBus.h"
#include "constants.h"


class Memory {
private:

public:
    Memory(AddressBus,DataBus);
void ReadMemory();
void WriteMemory();

};


#endif //EMULATOR_MEMORY_H
