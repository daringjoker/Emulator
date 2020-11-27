//
// Created by DaringJoker on 10/10/2020.
//

#include "RegisterPair.h"

RegisterPair::RegisterPair(Register &reg1, Register &reg2) :
        high{reg1}, low{reg2} {

}

void RegisterPair::set(Word value) {
    high.set((value >> 8));
    low.set(value & 0xff);
}

Word RegisterPair::get() {
    Word temp = 0;
    temp = high.get();
    temp = temp << 8;
    temp += low.get();
    return temp;
}
