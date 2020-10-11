
#include "FlagRegister.h"

bool FlagRegister::isSet_SignFlag() {
    return asStruct().signFlag==1U;
}

void FlagRegister::setSignFlag() {
    asStruct().signFlag=1u;
}

void FlagRegister::resetSignFlag() {
    asStruct().signFlag=0U;
}

bool FlagRegister::isSet_ZeroFlag() {
    return asStruct().zeroFlag==1u;
}

void FlagRegister::setZeroFlag() {
    asStruct().zeroFlag=1u;
}

void FlagRegister::resetZeroFlag() {
     asStruct().zeroFlag=0u;
}

bool FlagRegister::isSet_AuxCarryFlag() {
    return asStruct().auxCarryFlag==1u;
}

void FlagRegister::setAuxCarryFlag() {
    asStruct().auxCarryFlag=1u;
}

void FlagRegister::resetAuxCarryFlag() {
    asStruct().auxCarryFlag=0u;
}

bool FlagRegister::isSet_PairityFlag() {
    return asStruct().pairityFlag==1u;
}

void FlagRegister::setPairityFlag() {
    asStruct().pairityFlag=1u;
}

void FlagRegister::resetPairityFlag() {
    asStruct().pairityFlag=0u;
}

bool FlagRegister::isSet_CarryFlag() {
    return asStruct().carryFlag==1u;
}

void FlagRegister::setCarryFlag() {
    asStruct().carryFlag=1u;
}

void FlagRegister::resetCarryFlag() {
    asStruct().carryFlag=0u;

}

FlagBits& FlagRegister::asStruct() {
    auto* data=(FlagBits*)&value;
    return *data;
}
