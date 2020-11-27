//
// Created by DaringJoker on 9/23/2020.
//

#include "Memory.h"

void Memory::ReadMemory() {
    Word temp1 = addrbus.Read();
    databus.write(mem[temp1]);
}

void Memory::WriteMemory() {

    mem[addrbus.Read()] = databus.read();

}

Memory::Memory(AddressBus &addrbus, DataBus &databus) :
        addrbus{addrbus}, databus{databus} {
    mem = new Byte[0xffff + 10];
    for (int i = 0; i < 0xffff + 10; ++i) {
        mem[i] = 0;
    }
}
