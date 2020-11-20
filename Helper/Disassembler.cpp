//
// Created by DaringJoker on 11/19/2020.
//

#include "Disassembler.h"
#include "../InstructionSet/Instructions.h"


Disassembler::Disassembler(vector<Byte> Binary, Word address) {
    while (index < Binary.size()) {
        Byte opcode = Binary[index++];
        Instruction instr;
        try {
            instr = opcode2Instr.at(opcode);
        }
        catch (out_of_range &oor) {
            return;
        }
        AsmLine line;
        line.address = address + index-1;
        line.size = instr.Instruction_length;
        line.Assembly = instr.Pattern;
        line.rawBytes[0] = instr.opcode;
        char data[30]={0};
        if (line.size == 2) {
            line.rawBytes[1] = Binary[index++];
            sprintf(data,"%02Xh",line.rawBytes[1]);
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Data"), std::string(data));
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Port-address"), std::string(data));
        } else if (line.size == 3) {
            line.rawBytes[1] = Binary[index++];
            line.rawBytes[2] = Binary[index++];
            Word word=(((line.rawBytes[2]<<8)&0xff00)|(line.rawBytes[1]&0xff))&0xffff;
            sprintf(data,"%04Xh",word);
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Data"), std::string(data));
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Address"), std::string(data));
        }
        if (stripped) populateMap(line);
        Disassembled.push_back(line);
    }
    for (auto &line: Disassembled) {
        if (!labelMap[line.address].empty()) {
            line.label = labelMap[line.address];
        }
    }
}

void Disassembler::populateMap(AsmLine &line) {
    char labelname[30];
    if (regex_match(line.Assembly,
                    regex("^C(C|M|NC|NZ|PE|PO|P|Z)$|^J(NZ|MP|C|MP|NC|PO|PE|P|M|Z).*?$", std::regex_constants::icase))) {
        Word refaddr=(line.rawBytes[2]<<8)|line.rawBytes[1];
        if(labelMap[refaddr].empty()) {
            sprintf(labelname, "label%d", labelcounts++);
            labelMap[refaddr] = string(labelname);
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Label"), std::string(labelname));
        }
        else
        {
            line.Assembly = std::regex_replace(line.Assembly, std::regex("Label"), std::string(labelMap[refaddr]));
        }
    }
}
