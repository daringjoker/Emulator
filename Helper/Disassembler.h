//
// Created by DaringJoker on 11/19/2020.
//

#ifndef EMULATOR_DISASSEMBLER_H
#define EMULATOR_DISASSEMBLER_H
#include "../Core/DataTypes.h"
#include <vector>
#include <string>
#include <map>
#include <regex>

typedef struct AsmLine{
    char rawBytes[3];
    int size;
    string label;
    string comment;
    string Assembly;
    Word address;
}AsmLine;
class Disassembler {
private:
    bool stripped=true;
    int labelcounts=0;
    map<Word,string> labelMap;
    int index=0;
    void populateMap(AsmLine &line);
public:
    vector<AsmLine> Disassembled;
    explicit Disassembler(vector<Byte> Binary,Word address=0x4000);
};


#endif //EMULATOR_DISASSEMBLER_H
