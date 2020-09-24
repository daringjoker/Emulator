//
// Created by DaringJoker on 9/23/2020.
//

#include "AddressBus.h"
#include<iostream>

void AddressBus::Write(Word addr) {
    this->addr=addr;
}

Word AddressBus::Read() {
    return this->addr;
}
