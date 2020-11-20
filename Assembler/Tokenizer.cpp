//
// Created by DaringJoker on 9/23/2020.
//

#include "Tokenizer.h"

vector<sToken> Tokenizer::tokenize_line(string Line) {
    //strip the leading and trailing whitespace characters;
    Line=std::regex_replace(Line, std::regex("^\\s+"), std::string(""));
    Line=std::regex_replace(Line, std::regex("\\s+$"), std::string(""));
    auto * tokens=new vector<sToken>;
    if(Line.length()>0)
    {
        //remove the comments
        Line=strip_comment(Line);
        if (Line.length()>0){
            Line=std::regex_replace(Line, std::regex(","), std::string(" , "));
            vector<string>stoken=explode(Line);
            for(auto w : stoken){
               if (regex_match(w,regex("^[_A-Za-z][0-9_a-zA-Z]*?:$")))
                  tokens->push_back(sToken({w, sTokenType::dec_label}));
               else if(w==string(","))
                   tokens->push_back(sToken({w, sTokenType::comma}));
               else if(is_instruction(w))
                   tokens->push_back(sToken({w, sTokenType::instr}));
               else if(is_register(w))
                   tokens->push_back(sToken({w, sTokenType::reg}));
               else if(regex_match(w,regex("^[0-9]+$")))
                   tokens->push_back(sToken({w, sTokenType::dec_imm}));
               else if(regex_match(w,regex("^[0-9a-fA-F]+[hH]$")))
                   tokens->push_back(sToken({w, sTokenType::hex_imm}));
               else if(regex_match(w,regex("^[0-1]+[bB]$")))
                   tokens->push_back(sToken({to_string(stoi(w,0,2)), sTokenType::dec_imm}));
               else if(regex_match(w,regex("^[0-7]+[oO]$")))
                   tokens->push_back(sToken({to_string(stoi(w,0,8)), sTokenType::dec_imm}));
               else if(regex_match(w,regex("'.'")))
                   tokens->push_back(sToken({to_string((int)w[1]), sTokenType::dec_imm}));
               else
                   tokens->push_back(sToken({w, sTokenType::tbd}));
            }
        }
    }
    tokens->push_back(sToken({"", sTokenType::lineend}));
    return *tokens;
}

string Tokenizer::strip_comment(string& Line) {
    return std::regex_replace(Line, std::regex(";.*?$"), std::string(""));
}

 vector<string> Tokenizer::explode(const string& Line) {
    stringstream ss(Line);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    auto *vstrings=new vector<string>(begin, end);
    return *vstrings;
}
