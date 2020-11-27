//
// Created by DaringJoker on 9/23/2020.
//

#ifndef EMULATOR_TOKENIZER_H
#define EMULATOR_TOKENIZER_H

#include <string>
#include <vector>
#include <regex>
#include "../InstructionSet/Instructions.h"

using namespace std;
enum sTokenType {
    instr, hex_imm, dec_imm, bin_imm, oct_imm, ascii_imm, dec_label, ref_label, reg, tbd, comma, lineend
};
typedef struct {
    string tvalue;
    sTokenType type;
} sToken;

class Tokenizer {
public:
    Tokenizer() = default;

    static vector<string> explode(const string &Line);

    static vector<sToken> tokenize_line(string Line);

    static string strip_comment(string &Line);
};


#endif //EMULATOR_TOKENIZER_H
