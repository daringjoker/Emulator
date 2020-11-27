//
// Created by DaringJoker on 9/23/2020.
//

#include "AddressBus.h"

void AddressBus::Write(Word addr) {
    this->addr = addr;
}

Word AddressBus::Read() {
    return this->addr;
}
