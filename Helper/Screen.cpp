//
// Created by DaringJoker on 11/24/2020.
//

#include "Screen.h"

void Screen::printf(const char *format, ...) {
    char curline[4096]={0};
    va_list args;
    va_start (args, format);
    vsprintf (curline,format, args);
    buffer<<curline;
    va_end (args);
}

void Screen::flush() {
    cout<<buffer.str();
    buffer.str(string());
}
