#include <iostream>
#include "Assembler.h"
#include "ExecFileFormat/Execfile.h"
#include "../Helper/Argparse.h"
#include <regex>
void showMetaInfo(const Assembler &a);
void ListErrors(const Assembler &a);

bool verbose=false;
char header[] = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"\
                    "|                            8085 Assembler                         |\n"\
                    "| V(0.1)sept 2020                                   -BY PRS Company |\n"\
                    "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

char footer[] = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"\
                    "+++++++++++++++++++        Happy Learning          ++++++++++++++++++\n"\
                    "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
using namespace std;
int main(int argc,char ** argv) {
    Argparse parser("An Assembler for your 8085 assembly code.");
    parser.setHeader(header);
    parser.setFooter(footer);
    parser.AddArgument("f","filename",Argparse::Path,"Path of the file to be assembled",true,true);
    parser.AddArgument("o","outfile",Argparse::Path,"Path of the file on which the output is to be written");
    parser.AddArgument("n","no-output",Argparse::Boolean,"Supress the Output file ie only check for errors");
    parser.AddArgument("m","metadata",Argparse::Boolean,"Show the Metadata in the File being Assembled");
    parser.AddArgument("s","strip",Argparse::Boolean,"Strip all the debugging information eg Labels");
    parser.AddArgument("v","verbose",Argparse::Boolean,"Prints Most of operations for debugging metadata is implied");
    parser.ParseArgument(argc,argv);
    auto outfileinfo=parser.getArg("-o");
    auto infileinfo=parser.getArg("f");
    auto supressOutput=parser.getArg("-n").found;
    auto Metadata=parser.getArg("-m").found;
    auto strip=parser.getArg("-s").found;
    verbose=parser.getArg("-v").found;
    Assembler a;
    a.Assemble_file(infileinfo.Value);
    if (a.parser.errorlist.empty()) {
        if(Metadata||verbose) {
            cout<<header;
            if(verbose)
            {
                cout << "                       Debugging Output Verbose                      \n" << endl;
                cout << "            Inputfile     : "<<infileinfo.Value<<endl;
                cout<<  "            SupressOutput : "<<(supressOutput?"True":"False")<<endl;
                cout<<  "            ShowMetadata  : "<<(Metadata?"True":"True(Implied by verbose)")<<endl;
                cout<<  "            Strip Symbols : "<<(strip?"True":"False")<<endl;
                cout<<  "            Verbose mode  : "<<"True"<<endl;
                cout<<  "            output file   : "<<(outfileinfo.found?outfileinfo.Value:"Not supplied")<<endl;
                cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
            }
            showMetaInfo(a);
        }
        if(!supressOutput) {
            string outfilename;
            if (outfileinfo.found) {
                outfilename = outfileinfo.Value;
            } else {
               outfilename= std::regex_replace(infileinfo.Value, std::regex("\\.(.*?)$"), std::string(".85"));
            }
            Execfile outfile(a.assembled);
            if(!strip) outfile.labelmap = a.label2addr;
            outfile.Dumpfile(outfilename);
        }

    } else {
        cout<<header;
        if(verbose)
        {
            cout << "                       Debugging Output Verbose                      \n" << endl;
            cout << "            Inputfile     : "<<infileinfo.Value<<endl;
            cout<<  "            SupressOutput : "<<(supressOutput?"True":"False")<<endl;
            cout<<  "            ShowMetadata  : "<<(Metadata?"True":"True(Implied by verbose)")<<endl;
            cout<<  "            Strip Symbols : "<<(strip?"True":"False")<<endl;
            cout<<  "            Verbose mode  : "<<"True"<<endl;
            cout<<  "            output file   : "<<(outfileinfo.found?outfileinfo.Value:"Not supplied")<<endl;
            cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        }
        ListErrors(a);
    }
}

void ListErrors(const Assembler &a) {
    cout << "                         List of all Errors                         " << endl << endl;
    for (auto e:a.parser.errorlist) {
        if (e.line != string(""))
            cerr << "Error On line " << e.line_no << " : " << e.line << endl << "\t" << e.msg << endl << endl;
        else
            cerr << "Error On line " << e.line_no << " : " << e.msg << endl << endl;
    }
    cout << "\n\n              Please solve above errors and retry                  " << endl;
    cout<<footer;
}

void showMetaInfo(const Assembler &a)
{

    cout << "                       Labels Map of Executable                      \n" << endl;
    for (auto b:a.label2addr)
        printf("\t%4x : %s\n", b.second, b.first.c_str());
    cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "                       Memory Map of Executable                      " << endl << endl;
    int c = 0;
    printf("\t%4x : ", c + a.baseaddr);
    for (auto f:a.assembled) {
        c++;
        printf("%02x", f);
        if (c % 16 == 0) {
            if (a.assembled.size() > c)
                printf("\n\t%4x : ", c + a.baseaddr);
        } else
            printf(" ");
    }
    cout<<endl;
    cout<<footer;
}
