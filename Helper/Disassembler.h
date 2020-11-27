//
// Created by DaringJoker on 11/19/2020.
//

#ifndef EMULATOR_DISASSEMBLER_H
#define EMULATOR_DISASSEMBLER_H

#include "../Core/DataTypes.h"
#include "Grapher.h"
#include <vector>
#include <string>
#include <map>
#include <regex>

typedef struct AsmLine {
    char rawBytes[3];
    int size;
    string label;
    string arrows;
    string comment;
    string Assembly;
    Word address;
} AsmLine;
typedef struct arrow {
    Word from;
    Word to;
} Arrow;

class Disassembler {
private:
    vector<Byte> Binary;
    vector<Arrow> jumparrows;
    Word address;
    bool stripped = true;
    int labelcounts = 0;
    int index = 0;
    void populateMap(AsmLine &line);
public:

    Grapher grapher;

    vector<AsmLine> Disassembled;
    map<Word, string> labelMap;

    explicit Disassembler() = default;
    AsmLine Disassemble_line();
    explicit Disassembler(vector<Byte> Binary, Word address = 0x4000);
    int line_no(Word address);
    void loadLabelMap(map<string, Word> labelmap);

    void Disassemble();
};


#endif //EMULATOR_DISASSEMBLER_H
