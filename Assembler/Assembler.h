//
// Created by DaringJoker on 9/23/2020.
//

#ifndef EMULATOR_ASSEMBLER_H
#define EMULATOR_ASSEMBLER_H
#include "Parser.h"
#include "../Components/constants.h"
#include "../InstructionSet/Instructions.h"
#include<vector>
#include<map>
#include<set>
#include<string>

using namespace std;

class Assembler {
public:
    Word baseaddr;
    Word currentAddress;
    vector<Instr_line> parsed_data;
    Parser parser;
    vector<Byte> assembled;
    set<string>  unknownLabel;
    map<string,Word> label2addr;
    explicit Assembler(Word baseaddr=0x4000);
    void Assemble_file(string filename);
    void Assemble_text(string text);
private:
    void make_label_map();
    void assemble();
    void get_opcode();
    Byte opcode_of(string pattern);
};


#endif //EMULATOR_ASSEMBLER_H
