#ifndef EMULATOR_FLAGREGISTER_H
#define EMULATOR_FLAGREGISTER_H
#include "constants.h"
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
typedef union {
    Byte value;
    FlagBits bits;
} FlagContent;
class FlagRegister {
private:
    FlagContent content;
public:
    void setContent(Byte value);
    void getContent();
    bool getSignFlag();
    void setSignFlag();
    void resetSignFlag();
    bool getZeroFlag();
    void setZeroFlag();
    void resetZeroFlag();
    bool getAuxCarryFlag();
    void setAuxCarryFlag();
    void resetAuxCarryFlag();
    bool getPairityFlag();
    void setPairityFlag();
    void resetPairityFlag();
    bool getCarryFlag();
    void setCarryFlag();
    void resetCarryFlag();
};


#endif //EMULATOR_FLAGREGISTER_H
