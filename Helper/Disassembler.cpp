//
// Created by DaringJoker on 11/19/2020.
//

#include "Disassembler.h"

#include <utility>
#include "../InstructionSet/Instructions.h"


Disassembler::Disassembler(vector<Byte> Binary, Word address) {
    this->Binary=std::move(Binary);
    this->address=address;
}

void Disassembler::Disassemble() {
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
        line.address = address + index - 1;
        line.size = instr.Instruction_length;
        line.Assembly = instr.Pattern;
        line.rawBytes[0] = instr.opcode;
        char data[30]={0};
        if (line.size == 2) {
            line.rawBytes[1] = Binary[index++];
            sprintf(data,"%02Xh",line.rawBytes[1]);
            line.Assembly = regex_replace(line.Assembly, regex("Data"), string(data));
            line.Assembly = regex_replace(line.Assembly, regex("Port-address"), string(data));
        } else if (line.size == 3) {
            line.rawBytes[1] = Binary[index++];
            line.rawBytes[2] = Binary[index++];
            Word word=(((line.rawBytes[2]<<8)&0xff00)|(line.rawBytes[1]&0xff))&0xffff;
            sprintf(data,"%04Xh",word);
            line.Assembly = regex_replace(line.Assembly, regex("Data"), string(data));
            line.Assembly = regex_replace(line.Assembly, regex("Address"), string(data));
        }
        populateMap(line);
        Disassembled.push_back(line);
    }
    for (auto &line: Disassembled) {
//        line.arrows=string(allarrows.size()+1,' ');
        int columnwidth=allarrows.size()*2;
        int c=0;
        bool across=false;
        bool to=false;
        bool from=false;
        for(auto & arrow:allarrows)
        {
            bool cross=false;
            if(line.address==arrow.from || line.address==arrow.to)
            {
                across=true;
                if(!to) to=line.address==arrow.to;
                if(!from) from=line.address==arrow.from;
                if(line.address==max(arrow.to,arrow.from))line.arrows+="`-";
                else line.arrows+=",-";
                continue;
            }
            if(across&&line.address<max(arrow.from,arrow.to)&&line.address>min(arrow.from,arrow.to))
            {
                cross=true;
            }
            if(cross)
                line.arrows+="+-";
            else if(across)
                line.arrows+="--";
            else if(line.address<max(arrow.from,arrow.to)&&line.address>min(arrow.from,arrow.to))
            {
                line.arrows+="| ";
            }else
            {
                line.arrows+="  ";
            }
                c++;
        }
        if(across){
            if(to&&from)
                line.arrows+="-<<->";
            else if(to)
                line.arrows+="---->";
            else line.arrows+="--<<<";
        }
        else line.arrows+=string(columnwidth-line.arrows.size()+5,' ');
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
        allarrows.push_back({line.address,refaddr});
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

void Disassembler::loadLabelMap(map<string,Word> label2addr) {
    this->stripped=false;
    for(auto item:label2addr)
    {
        labelMap[item.second]=item.first;
    }
}
