//
// Created by DaringJoker on 9/23/2020.
//created by  uniqueram   on 9/24/2020

#ifndef EMULATOR_PORTS_H
#define EMULATOR_PORTS_H

#include "AddressBus.h"
#include "DataBus.h"
#include "constants.h"

class Ports {
private:
    AddressBus &addrbus;
    DataBus &databus;
public:
    Byte *place;

    Ports(AddressBus &addrbus, DataBus &databus);

    void ReadPort();

    void WritePort();
};


#endif //EMULATOR_PORTS_H
