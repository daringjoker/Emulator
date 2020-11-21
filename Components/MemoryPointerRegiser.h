//
// Created by DaringJoker on 11/21/2020.
//

#ifndef EMULATOR_MEMORYPOINTERREGISER_H
#define EMULATOR_MEMORYPOINTERREGISER_H
#include "Register.h"
#include "RegisterPair.h"
#include "AddressBus.h"
#include "DataBus.h"
#include "Memory.h"

class MemoryPointerRegiser:public Register {
    RegisterPair& HL;
    AddressBus& Abus;
    DataBus& Dbus;
    Memory& Ram;
public:
    MemoryPointerRegiser(Memory& ram,AddressBus& abus,DataBus& dbus,RegisterPair& HL);
    void set(Byte data) override;
    Byte get() override;
};


#endif //EMULATOR_MEMORYPOINTERREGISER_H
