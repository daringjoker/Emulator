//
// Created by DaringJoker on 9/23/2020.
//

#include "Ports.h"

Ports::Ports(AddressBus &addrbus, DataBus &databus):
addrbus {addrbus},databus{databus}
{
    place = new Byte[0xff+10];
    for (int i = 0; i < 0xff + 10; ++i) {
        place[i]=0;
    }

}

void Ports::ReadPort() {
    Word temp= addrbus.Read();
    Byte portaddr=temp&0xff;
    databus.write(place[portaddr]);

}

void Ports::WritePort() {
    Word temp= addrbus.Read();
    Byte portaddr=temp&0xff;
    place[portaddr]=databus.read();

}
