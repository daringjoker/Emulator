//
// Created by DaringJoker on 11/21/2020.
//
#include "../Emulator.h"

void Emulator::PUSH(Decoded_Instr instr)
{
    char mnemonic[50],arg1[20];
    sscanf(instr.stencil.c_str(),"%s %s",mnemonic,arg1);

    Word sp=SP.get();
    Word RP=getRegPairByName(arg1).get();
    Word RPh=(RP&0xff00)>>8;
    Word RPl=RP&0xff;

    Abus.Write(sp-1);
    Dbus.write(RPh);
    Ram.WriteMemory();

    Abus.Write(sp-2);
    Dbus.write(RPl);
    Ram.WriteMemory();

    sp=sp-2;
    SP.set(sp);
}

void Emulator::POP(Decoded_Instr instr)
{
    char mnemonic[50],arg1[20];
    sscanf(instr.stencil.c_str(),"%s %s",mnemonic,arg1);

    Word sp=SP.get();

    Abus.Write(sp);
    Ram.ReadMemory();
    Byte RPl=Dbus.read();

    Abus.Write(sp+1);
    Ram.ReadMemory();
    Byte RPh=Dbus.read();

    Word RP=(RPh<<8)+RPl;
    getRegPairByName(arg1).set(RP);

    sp=sp+2;
    SP.set(sp);
}
void Emulator::XTHL(Decoded_Instr instr)
{
    Word sp=SP.get();

    Abus.Write(sp);
    Ram.ReadMemory();
    Byte RPl=Dbus.read();

    Abus.Write(sp-1);
    Ram.ReadMemory();
    Byte RPh=Dbus.read();

    Word wordAtTopOfStack= (RPh << 8) + RPl;
    Byte h=H.get();
    Byte l=L.get();

    HL.set(wordAtTopOfStack);

    Abus.Write(sp-1);
    Dbus.write(h);
    Ram.WriteMemory();

    Abus.Write(sp-2);
    Dbus.write(l);
    Ram.WriteMemory();

}
void Emulator::SPHL(Decoded_Instr instr)
{
    Word hl=HL.get();
    SP.set(hl);
}