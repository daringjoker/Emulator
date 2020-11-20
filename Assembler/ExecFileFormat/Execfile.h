//
// Created by DaringJoker on 11/19/2020.
//

#ifndef EMULATOR_EXECFILE_H
#define EMULATOR_EXECFILE_H
#include <cstdint>
#include <vector>
#include <map>
#include <fstream>
#include <regex>
#include "../../Components/constants.h"

using namespace std;


class Execfile {
public:
    typedef struct LabelDirectoryHeader{
        int16_t size;
        int16_t address;
        int16_t length;
    }__attribute__((packed)) LabelDirectoryHeader;
    typedef struct FileHeader{
        char signature[5]={'P','R','S','8','5'};
        int16_t RawSize;
        int8_t  Stripped=false;
        int16_t NoOfLabels;
        int16_t LabelDirectoryAddress;
        int16_t StartOfcode;
        int16_t BaseAddress=0x4000;
    }__attribute__((packed)) Fileheader;
    vector<Byte> Binarycode;
    Word Baseaddr=0x4000;
    FileHeader header;
    map<string,Word> labelmap;
    explicit Execfile(vector<Byte>&BinaryCode,Word Baseaddr=0x4000);
    explicit Execfile(string filename);
    explicit Execfile(char* Memory,Word baseaddr,Word endaddr);
    void Dumpfile(string filename);
};


#endif //EMULATOR_EXECFILE_H
