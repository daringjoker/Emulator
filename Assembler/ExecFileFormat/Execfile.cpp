//
// Created by DaringJoker on 11/19/2020.
//

#include "Execfile.h"

Execfile::Execfile(vector<Byte> &BinaryCode, Word Baseaddr) {
    this->Baseaddr = Baseaddr;
    this->Binarycode = BinaryCode;
}

Execfile::Execfile(string filename) {
    ifstream infile(filename, ios::in | ios::binary);
    infile.read((char *) &header, sizeof(header));
    int codesize = header.LabelDirectoryAddress - header.StartOfcode;
    char *codebuffer = new char[codesize + 10];
    infile.read(codebuffer, codesize);
    for (int i = 0; i < codesize; ++i) {
        Binarycode.push_back(codebuffer[i]);
    }
    if (!header.Stripped) {
        for (int i = 0; i < header.NoOfLabels; ++i) {
            auto *ldh = new LabelDirectoryHeader;
            infile.read((char *) ldh, sizeof(LabelDirectoryHeader));
            char *str = new char[ldh->length + 10];
            infile.read(str, ldh->length);
            str[ldh->length] = 0;
            labelmap[string(str)] = ldh->address;
        }
    }

}

Execfile::Execfile(char *Memory, Word baseaddr, Word endaddr) {

}

void Execfile::Dumpfile(string filename) {
    if (!regex_match(filename, regex("^(.*?)\\.85$", std::regex_constants::icase)))
        filename = filename + ".85";
    header.BaseAddress = this->Baseaddr;
    header.NoOfLabels = labelmap.size();
    header.StartOfcode = sizeof(header);
    header.LabelDirectoryAddress = header.StartOfcode + Binarycode.size();
    if (header.NoOfLabels == 0) header.Stripped = true;
    char *buffer = new char[this->Binarycode.size() + 10];
    for (int i = 0; i < Binarycode.size(); ++i) {
        buffer[i] = Binarycode[i];
    }
    char *Label_Buffer = new char[1024];
    int bufferPointer = 0;
    if (!labelmap.empty()) {
        for (const auto &item:labelmap) {
            LabelDirectoryHeader ldh;
            ldh.size = sizeof(ldh) + item.first.size();
            ldh.length = item.first.size();
            ldh.address = item.second;
            memcpy((void *) (Label_Buffer + bufferPointer), &ldh, sizeof(ldh));
            bufferPointer += sizeof(ldh);
            memcpy((void *) (Label_Buffer + bufferPointer), item.first.c_str(), item.first.size());
            bufferPointer += ldh.length;
        }

    }
    header.RawSize = header.LabelDirectoryAddress + bufferPointer;
    ofstream outfile(filename, ios::out | ios::binary);
    outfile.write((char *) &header, sizeof(header));
    outfile.write(buffer, Binarycode.size());
    if (!labelmap.empty()) {
        outfile.write(Label_Buffer, bufferPointer);
    }
    outfile.close();
}

bool Execfile::isExecutable(string filename) {
    ifstream myFile(filename, ios::in | ios::binary);
    char signature[5] = {0};
    myFile.read(signature, 5);
    if (signature[0] == 'P' && signature[1] == 'R' && signature[2] == 'S' && signature[3] == '8' && signature[4] == '5')
        return true;
    else
        return false;
}