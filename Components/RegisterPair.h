//
// Created by DaringJoker on 10/10/2020.
//

#ifndef EMULATOR_REGISTERPAIR_H
#define EMULATOR_REGISTERPAIR_H
#include "Register.h"
#include "constants.h"
class RegisterPair {
protected:
    Register& high;
    Register& low;
public:
    RegisterPair(Register& reg1,Register& reg2);
    void set(Word value);
    Word get();
};


#endif //EMULATOR_REGISTERPAIR_H
