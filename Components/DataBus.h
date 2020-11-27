//
// Created by DaringJoker on 9/23/2020.
//created by uniqueram on 9/24/2020

#ifndef EMULATOR_DATABUS_H
#define EMULATOR_DATABUS_H

#include "constants.h"

class DataBus {
private:
    Byte data;
public:
    DataBus() = default;

    void write(Byte data);

    Byte read();
};


#endif //EMULATOR_DATABUS_H
