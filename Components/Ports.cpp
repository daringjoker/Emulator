//
// Created by DaringJoker on 9/23/2020.
//

#include "Ports.h"

Ports::Ports(AddressBus &addrbus, DataBus &databus):
addrbus {addrbus},databus{databus}
{
    place = new Byte[0xff+10];

}

void Ports::ReadPort() {
    Word temp= addrbus.Read();
    databus.write(place[temp]);


}

void Ports::WritePort() {
    place[addrbus.Read()]=databus.read();

}
