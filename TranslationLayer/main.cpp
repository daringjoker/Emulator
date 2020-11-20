#include "../Core/emulator.h"
#include "../Helper/Argparse.h"
#include "../Helper/Disassembler.h"
#include "../Assembler/Assembler.h"

int main(int argc, char** argv)
{
//    Argparse parser("A commandline Emulator for your 8085 assembly code");
//    parser.AddArgument("f","filename",Argparse::Path,"Path of the file that is to be loaded",true,true);
//    parser.AddArgument("r","run",Argparse::Boolean,"Just Run the file don't load into repl mode");
//    parser.AddArgument("w","write",Argparse::Boolean,"Open the file in write mode for patching");
//    parser.AddArgument("p","prompt",Argparse::String,"Set the prompt to the given string");
//    parser.ParseArgument(argc,argv);
Assembler assembler;
assembler.Assemble_file("doit.asm");
Disassembler dis(assembler.assembled);
for(auto & line : dis.Disassembled)
{
    if(line.label.empty())
        printf("%04X~ %-20s  %-20s\n",line.address,"",line.Assembly.c_str());
    else
        printf("%04X~ %20s: %-20s\n",line.address,line.label.c_str(),line.Assembly.c_str());
}
}

