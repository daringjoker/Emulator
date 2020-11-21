//
// Created by DaringJoker on 11/21/2020.
//
#include "../Emulator.h"
void Emulator::IN(Decoded_Instr instr)
{
    Abus.Write(instr.dataByte);
    ports.ReadPort();
    Byte data=Dbus.read();
    A.set(data);
}
void Emulator::OUT(Decoded_Instr instr)
{
    Byte data=A.get();
    Abus.Write(instr.dataByte);
    Dbus.write(data);
    ports.WritePort();
}