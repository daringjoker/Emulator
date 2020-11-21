//
// Created by DaringJoker on 11/21/2020.
//

#include "MemoryPointerRegiser.h"

MemoryPointerRegiser::MemoryPointerRegiser(Memory& ram,AddressBus &abus, DataBus &dbus, RegisterPair &HL):
Register::Register("M"),Abus{abus},Dbus{dbus},HL{HL},Ram{ram}
{

}

void MemoryPointerRegiser::set(Byte data) {
    Word pointer=HL.get();
    Abus.Write(pointer);
    Dbus.write(data);
    Ram.WriteMemory();
}

Byte MemoryPointerRegiser::get() {
    Word pointer=HL.get();
    Abus.Write(pointer);
    Ram.ReadMemory();
    Byte temp=Dbus.read();
    return temp;
}
