#include "../Core/Emulator.h"
#include "../Helper/Argparse.h"
#include "../Helper/Disassembler.h"
#include "../Assembler/Assembler.h"
#include "../Core/Emulator.h"
#include "../Assembler/ExecFileFormat/Execfile.h"

void DisassembleFile();
void execute(string& command);

void printDebugStatus();

void printRegisters();

Assembler assembler;
Emulator emulator;
Disassembler disassembler;
Execfile execfile;
namespace config
{
    string filename;
    string prompt;
    bool run;
    bool disassemble;
    bool jsonOutput;
    bool write;
    bool isValidExecutable;
    bool running=true;
}
int main(int argc, char** argv)
{
    Argparse parser("A commandline Emulator for your 8085 assembly code");
    parser.AddArgument("f","filename",Argparse::Path,"Path of the file that is to be loaded",true,true);
    parser.AddArgument("r","run",Argparse::Boolean,"Run the file at the beginning of the debugging");
    parser.AddArgument("w","write",Argparse::Boolean,"Open the file in write mode for patching");
    parser.AddArgument("j","json-output",Argparse::Boolean,"Open the file in write mode for patching");
    parser.AddArgument("d","disassemble",Argparse::Boolean,"Only print the disassembly of the file");
    parser.AddArgument("p","prompt",Argparse::String,"Set the prompt to the given string");
    parser.ParseArgument(argc,argv);
    config::run=parser.getArg("-r").found;
    config::disassemble=parser.getArg("-d").found;
    config::filename=parser.getArg("f").Value;
    config::prompt=parser.getArg("-p").found?parser.getArg("-p").Value:"PRS>> ";
    config::jsonOutput=parser.getArg("-j").found;
    config::write=parser.getArg("-w").found;
    config::isValidExecutable=Execfile::isExecutable(config::filename);
    emulator.reset();
    if(config::isValidExecutable) {
        execfile=Execfile(config::filename);
        disassembler=Disassembler(execfile.Binarycode);
        if(!execfile.header.Stripped)
            disassembler.loadLabelMap(execfile.labelmap);
        disassembler.Disassemble();
        emulator.load_binary(execfile.Binarycode);
    }
    else
    {
        assembler.Assemble_file(config::filename);
        emulator.load_binary(assembler.assembled);
        disassembler=Disassembler(assembler.assembled);
        disassembler.loadLabelMap(assembler.label2addr);
        disassembler.Disassemble();
    }
    if(config::disassemble){
        DisassembleFile();
        exit(0);
    }
    while(config::running)
    {
        char cmd[1024]={0};
       cout<<config::prompt;
       cin.getline(cmd,1024);
       string command(cmd);
       execute(command);
    }
}

void execute(string& command) {
    switch(tolower(command[0]))
    {
        case 'd':
            DisassembleFile();
            break;
        case 'q':
            config::running=false;
            exit(0);
        case 'c':
            if(command.size()>1&&tolower(command[1])=='l')
            {
                if (system("CLS")) system("clear");
            }
            else{
                emulator.run();
                printDebugStatus();
            }
            break;
        case 'n':
            emulator.single_step();
            printDebugStatus();
            break;
        case 'r':
            if(command==string("reset"))
            {
                emulator.reset();
            }
            else
            {
                printRegisters();
            }
            break;
        case 'b':
        {
            switch(tolower(command[1])) {
                case 'c':
                    emulator.breakpoints.clear();
                    break;
                case 'l':
                    for (auto item:emulator.breakpoints) {
                        printf("\tBreakpoint at %04XH \n", item & 0xffff);
                    }
                    break;
                case 'a': {
                    char addr[100];
                    char _[100];
                    sscanf(command.c_str(), "%s %s", _, addr);
                    string address(addr);
                    if (address.length() <= 4) {
                        Word realAddr = stoi(address, 0, 16);
                        emulator.addbreakpoint(realAddr);
                    }
                }
                    break;
                case 'd': {
                    char addr[100];
                    char _[100];
                    sscanf(command.c_str(), "%s %s", _, addr);
                    string address(addr);
                    if (address.length() <= 4) {
                        Word realAddr = stoi(address, 0, 16);
                        emulator.rembreakpoint(realAddr);
                    }
                }
                    break;
                case '?': {
                    printf("\t%-30s\t\t%s\n", "ba ADDRESS", "Adds new breakpoint at specified address");
                    printf("\t%-30s\t\t%s\n", "bc", "Clears all the breakpoints.");
                    printf("\t%-30s\t\t%s\n", "bd ADDRESS", "Deletes all the breakpoints at specified address");
                    printf("\t%-30s\t\t%s\n", "bl", "list all the breakpoints");
                    printf("\t%-30s\t\t%s\n", "b?", "Displays this help ");

                }
                    break;
            }
            }
    }
}

void printRegisters() {
    Byte a=emulator.A.get();
    Byte b=emulator.B.get();
    Byte c=emulator.C.get();
    Byte d=emulator.D.get();
    Byte e=emulator.E.get();
    Byte h=emulator.H.get();
    Byte l=emulator.L.get();
    Word sp=emulator.SP.get();
    Word pc=emulator.PC.get();
    printf("\t REGISTERS:-\n");
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","A",a,a,a);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","B",b,b,b);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","C",c,c,c);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","D",d,d,d);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","E",e,e,e);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","H",h,h,h);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","L",l,l,l);
}

void printDebugStatus() {

}

void DisassembleFile() {
        for(auto & item:disassembler.Disassembled)
        {
            char bytes[20]={0};
            for (int i = 0; i < item.size; ++i) {
                sprintf(bytes,"%s %02X",bytes,item.rawBytes[i]&0xff);
            }
            char brk=(emulator.breakpoints.count(item.address)>0)?'*':' ';
            const char *pc=((emulator.PC.get()==item.address)?"PC=>":"    ");
            char labelsep=(item.label.empty()?' ':':');
            printf("%s|%s%c%4XH | %-9s %20s%c %-20s\n",item.arrows.c_str(),pc,brk,item.address,bytes,item.label.c_str(),labelsep,item.Assembly.c_str());
        }
}
