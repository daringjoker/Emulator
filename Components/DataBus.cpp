//created by uniqueram on 9/24/2020.
// Created by DaringJoker on 9/23/2020.

#include "DataBus.h"

void DataBus::write(Byte data) {
    this->data=data;

}

Byte DataBus::read() {
    return this->data;
}
