//
// Created by DaringJoker on 9/23/2020.
//

#include "Parser.h"
#include <iostream>
vector<Instr_line> Parser::parse_file(string filename) {
ifstream infile(filename);
stringstream buffer;
buffer<<infile.rdbuf();
return parse_text(buffer.str());
}

vector<Instr_line> Parser::parse_text(string text) {
//    text=std::regex_replace(text, std::regex(":[\\s\\n\\r]+"), std::string(": "));
    stringstream ss(text);
    string line;
    int line_number=0;
    auto parsed_data=new vector<Instr_line>;
    while(getline(ss,line))
    {
        line_number++;
        auto data=parse_line(line, line_number);
        parsed_data->push_back(data);
    }
    return *parsed_data;
}

Instr_line Parser::parse_line(string line, int line_no) {
    vector<sToken> tokens=Tokenizer::tokenize_line(line);
    auto * effInstr=new Instr_line;
    int token_index=0;
    if(tokens[token_index].type==sTokenType::lineend){
        effInstr->num_token=token_index;
        return *effInstr;
    }
    if(pending_label&&tokens[token_index].type!=sTokenType::dec_label)
    {
        pending_label=false;
        effInstr->to_be_labelled = true;
        effInstr->labelname = std::regex_replace(label, std::regex(":"), std::string(""));
    }
    else if(pending_label && tokens[token_index].type==sTokenType::dec_label)
        reg_err("Multiple label name for same line ")

    if(tokens[token_index].type==sTokenType::dec_label) {
        effInstr->to_be_labelled = true;
        effInstr->labelname = std::regex_replace(tokens[token_index].tvalue, std::regex(":"), std::string(""));
        token_index++;
        if(tokens[token_index].type==sTokenType::lineend){
            pending_label=true;
            label=effInstr->labelname;
            effInstr->num_token=0;
            return *effInstr;
        }
    }
    if(tokens[token_index].type==sTokenType::instr) {
        effInstr->instruct = tokens[token_index].tvalue;
        token_index++;
        if(tokens[token_index].type==sTokenType::lineend){
            effInstr->num_token=token_index;
            return *effInstr;
        }
    }
    else
        reg_err("Expected an instruction or a Label at the beginning of the line")
    if(tokens[token_index].type==sTokenType::reg||tokens[token_index].type==sTokenType::hex_imm||
    tokens[token_index].type==sTokenType::dec_imm||tokens[token_index].type==sTokenType::tbd) {
        effInstr->argument1 = tokens[token_index].tvalue;
        effInstr->arg1_type=tokens[token_index].type;
        token_index++;
        effInstr->num_args++;
        if(tokens[token_index].type==sTokenType::lineend){
            effInstr->num_token=token_index;
            return *effInstr;
        }
    }
    else
        reg_err("The first argument can only be an immediate,register or a label")
    if(tokens[token_index].type==sTokenType::comma) {
        token_index++;
        if(tokens[token_index].type==sTokenType::reg||tokens[token_index].type==sTokenType::hex_imm||
           tokens[token_index].type==sTokenType::dec_imm) {
            effInstr->argument2 = tokens[token_index].tvalue;
            effInstr->arg2_type=tokens[token_index].type;
            token_index++;
            effInstr->num_args++;
            if(tokens[token_index].type==sTokenType::lineend){
                effInstr->num_token=token_index;
                return *effInstr;
            }
            else
                reg_err("An Instruction can have at most 2 Arguments in 8085 microprocessor")
        }
        else if(tokens[token_index].type==sTokenType::lineend)
            reg_err("Second Argument is expected after the comma")
        else
            reg_err("The second argument can only be an immediate or a register")
    }
    else
        reg_err("A comma separator is expected between the arguments")

}

void Parser::register_error(string msg, string line, int line_no) {
    errorlist.push_back({msg,line_no,line});
}
