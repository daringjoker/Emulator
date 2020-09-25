//
// Created by DaringJoker on 9/23/2020.
//

#include "Memory.h"
#include "AddressBus.h"
#include "DataBus.h"

DataBus db;
AddressBus ab;
Word temp1,temp2;
Memory::Memory(AddressBus, DataBus) {
mem = new char [256];
}

void Memory::ReadMemory() {
 temp1= ab.Read();
db.write(mem[temp1]);
}

void Memory::WriteMemory() {

   mem[ab.Read()]=db.read();

}
