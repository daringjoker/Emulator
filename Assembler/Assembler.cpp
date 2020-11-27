//
// Created by DaringJoker on 9/23/2020.
//

#include "Assembler.h"
#include <iostream>

using namespace std;

Assembler::Assembler(Word baseaddr) {
    this->baseaddr = baseaddr;
    currentAddress = baseaddr;
}

void Assembler::make_label_map() {
    for (auto &instr:parsed_data) {
        if (instr.to_be_labelled) {
            label2addr[instr.labelname] = currentAddress;
        }
        if (regex_match(instr.instruct, regex("^C(C|M|NC|NZ|PE|PO|P|Z)$|^J(NZ|MP|C|MP|NC|PO|PE|P|M|Z)$",
                                              std::regex_constants::icase))) {
            if (instr.arg1_type == tbd)
                instr.arg1_type = sTokenType::ref_label;
            else
                parser.register_error("A label was Expected but couldnot get it","",0);
            if (label2addr.count(instr.argument1) == 1) {
                instr.immediate_word = label2addr[instr.argument1];
            } else {
                unknownLabel.insert(instr.argument1);
            }
        }
        auto gen = make_generic(instr.instruct);
        currentAddress += gen.size;
        instr.size = gen.size;
    }
    currentAddress = baseaddr;
    int l = 0;
    for (auto &instr:parsed_data) {
        l++;
        if (regex_match(instr.instruct, regex("^C(C|M|NC|NZ|PE|PO|P|Z)$|^J(NZ|MP|C|MP|NC|PO|PE|P|M|Z)$",
                                              std::regex_constants::icase))) {
            if (label2addr.count(instr.argument1) == 1) {
                instr.immediate_word = label2addr[instr.argument1];
                unknownLabel.erase(instr.argument1);
            } else {
                parser.register_error(string("undefined label ") + instr.argument1, "", l);
            }
        }
    }

}

void Assembler::Assemble_file(string filename) {
    this->parsed_data = parser.parse_file(filename);
    assemble();
}

void Assembler::Assemble_text(string text) {
    this->parsed_data = parser.parse_text(text);
    assemble();
}

void Assembler::assemble() {
    make_label_map();
    get_opcode();
    for (auto &instr:parsed_data) {
        if (instr.num_token > 0) {
            assembled.push_back(instr.opcode);
            switch (instr.size) {
                case 1:
                    break;
                case 2:
                    assembled.push_back(instr.immediate_byte);
                    break;
                case 3:
                    assembled.push_back((Byte) (instr.immediate_word & 0xff));
                    assembled.push_back((Byte) ((instr.immediate_word >> 8) & 0xff));
                    break;
            }
        }
    }
}

void Assembler::get_opcode() {
    int line = 0;
    Word imm;
    for (auto &instr:parsed_data) {
        line++;
        if (regex_match(instr.instruct, regex("^RST$", std::regex_constants::icase))) {
            char ins[10];
            sprintf(ins, "RST %s", instr.argument1.c_str());
            instr.instruct = string(ins);
            instr.num_token--;
            instr.num_args--;
        }
        if (instr.num_token > 0) {
            string pattern = instr.instruct;
            if (instr.num_args > 0) {
                pattern += string(" ");
                switch (instr.arg1_type) {
                    case ref_label:
                        pattern += string("Label");
                        break;
                    case hex_imm:
                        imm = stoi(std::regex_replace(instr.argument1, std::regex("[hH]"), std::string("")),
                                   reinterpret_cast<size_t *>(0), 16);
                        if (instr.size == 2)instr.immediate_byte = imm;
                        else if (instr.size == 3)instr.immediate_word = imm;
                        else parser.register_error("unexpected immediate value find;", "", line);
                        pattern += string("Data");
                        break;
                    case dec_imm:
                        imm = stoi(instr.argument1);
                        if (instr.size == 2)instr.immediate_byte = imm;
                        else if (instr.size == 3)instr.immediate_word = imm;
                        else parser.register_error("unexpected immediate value find;", "", line);
                        pattern += string("Data");
                        break;
                    case reg:
                        pattern += instr.argument1;
                        break;
                }
            }
            if (instr.num_args == 2) {
                pattern += string(", ");
                switch (instr.arg2_type) {
                    case hex_imm:
                        imm = stoi(std::regex_replace(instr.argument2, std::regex("[hH]"), std::string("")),
                                   reinterpret_cast<size_t *>(0), 16);
                        if (instr.size == 2)instr.immediate_byte = imm;
                        else if (instr.size == 3)instr.immediate_word = imm;
                        else parser.register_error("unexpected immediate value find;", "", line);
                        pattern += string("Data");
                        break;
                    case dec_imm:
                        imm = stoi(instr.argument2);
                        if (instr.size == 2)instr.immediate_byte = imm;
                        else if (instr.size == 3)instr.immediate_word = imm;
                        else parser.register_error("unexpected immediate value find;", "", line);
                        pattern += string("Data");
                        break;
                    case reg:
                        pattern += instr.argument2;
                        break;
                }
            }
            instr.opcode = opcode_of(pattern);
        }
    }
}


Byte Assembler::opcode_of(string pattern) {
    transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
    return Pattern2opcode[pattern];
}
