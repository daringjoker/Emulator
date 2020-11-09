//
// Created by DaringJoker on 11/9/2020.
//

#include "emulator.h"
Byte emulator::fetchByte() {
    Word address=this->PC.get();
    this->Abus.Write(address);
    this->Ram.ReadMemory();
    Byte value=this->Dbus.read();
    this->PC.set(address+1);
    return value;
}

Word emulator::fetchWord() {
  Word lvalue=fetchByte();
  Word hvalue=fetchByte();
  return ((hvalue<<8)|lvalue);
}