#ifndef EMULATOR_FLAGREGISTER_H
#define EMULATOR_FLAGREGISTER_H
#include "constants.h"
#include "Register.h"
typedef struct{
    unsigned int signFlag:1;
    unsigned int zeroFlag:1;
    unsigned int nc1:1;
    unsigned int auxCarryFlag:1;
    unsigned int nc2:1;
    unsigned int pairityFlag:1;
    unsigned int nc3:1;
    unsigned int carryFlag:1;
} FlagBits;
class FlagRegister:Register{
private:
    FlagBits& asStruct();
public:
    bool isSet_SignFlag();
    void setSignFlag();
    void resetSignFlag();
    bool isSet_ZeroFlag();
    void setZeroFlag();
    void resetZeroFlag();
    bool isSet_AuxCarryFlag();
    void setAuxCarryFlag();
    void resetAuxCarryFlag();
    bool isSet_PairityFlag();
    void setPairityFlag();
    void resetPairityFlag();
    bool isSet_CarryFlag();
    void setCarryFlag();
    void resetCarryFlag();
};


#endif //EMULATOR_FLAGREGISTER_H
