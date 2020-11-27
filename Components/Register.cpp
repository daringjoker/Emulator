//
// Created by DaringJoker on 9/15/2020.
//

#include "Register.h"

Byte Register::get() {
    return this->value;
}

void Register::set(Byte value) {
    this->value = value;
}

Register::Register(std::string regname) {
    name = regname;

}

