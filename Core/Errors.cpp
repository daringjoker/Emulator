//
// Created by DaringJoker on 11/9/2020.
//
#include <utility>

#include "emulator.h"
void emulator::record_error(string errorstring,Word address,Errorclass errclass)
{
    this->Error_list.push_back({std::move(errorstring),address,errclass});
}
