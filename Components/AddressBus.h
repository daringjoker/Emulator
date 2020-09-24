//
// Created by DaringJoker on 9/23/2020.
//

#ifndef EMULATOR_ADDRESSBUS_H
#define EMULATOR_ADDRESSBUS_H
#include "constants.h"

class AddressBus {
private:

    Word addr;
public:
    void ReadAddress(Word addr);
    void WriteAddress();


};


#endif //EMULATOR_ADDRESSBUS_H
