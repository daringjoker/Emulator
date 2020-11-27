//
// Created by DaringJoker on 9/23/2020.
//

#ifndef EMULATOR_PARSER_H
#define EMULATOR_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Tokenizer.h"

using namespace std;

typedef struct {
    bool to_be_labelled = false;
    string instruct;
    string labelname;
    int num_args = 0;
    string argument1;
    string argument2;
    sTokenType arg1_type;
    sTokenType arg2_type;
    int size;
    Byte opcode;
    Byte immediate_byte;
    Word immediate_word;
    int num_token = 0;
} Instr_line;

typedef struct {
    string msg;
    int line_no;
    string line;
} Error;
#define reg_err(msg) {\
                        register_error(msg,line,line_no); \
                        return *effInstr;\
                    }


class Parser {
public:
    vector<Error> errorlist;

    Parser() = default;

    string label;
    bool pending_label = false;

    vector<Instr_line> parse_file(string filename);

    vector<Instr_line> parse_text(string text);

    Instr_line parse_line(string line, int line_no);

    void register_error(string msg, string line, int line_no);
};


#endif //EMULATOR_PARSER_H
