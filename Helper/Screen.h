//
// Created by DaringJoker on 11/24/2020.
//

#ifndef EMULATOR_SCREEN_H
#define EMULATOR_SCREEN_H
#include <sstream>
#include <iostream>
#include <cstdarg>
using namespace std;
class Screen {
    inline static stringstream buffer=stringstream();
public:
    static void printf(const char* format,...);
    static void flush();
};


#endif //EMULATOR_SCREEN_H
