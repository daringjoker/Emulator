//
// Created by DaringJoker on 9/15/2020.
//

#ifndef EMULATOR_REGISTERS_H
#define EMULATOR_REGISTERS_H
#include "constants.h"

class Register {
private:
    Byte value;
public:
    Register()=default;
    Word get();
    void set(Word value);

};


#endif //EMULATOR_REGISTERS_H
