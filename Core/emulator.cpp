//
// Created by DaringJoker on 11/7/2020.
//

#include "emulator.h"

void emulator::load_binary() {

}

void emulator::single_step() {

}

void emulator::run() {

}

void emulator::Reset() {
    this->PC.set(this->start_of_code);
    this->PSW.set(0);
    this->BC.set(0);
    this->DE.set(0);
    this->HL.set(0);
    this->SP.set(0xffff);
}
