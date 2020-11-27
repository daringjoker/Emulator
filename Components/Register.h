//
// Created by DaringJoker on 9/15/2020.
//

#ifndef EMULATOR_REGISTERS_H
#define EMULATOR_REGISTERS_H

#include<string>
#include "constants.h"

class Register {
protected:
    Byte value;
    std::string name;
public:
    explicit Register(std::string);

    Register();

    virtual Byte get();

    virtual void set(Byte value);

};


#endif //EMULATOR_REGISTERS_H
