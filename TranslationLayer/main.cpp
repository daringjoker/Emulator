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

void printMemoryDump(string command);

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
        case 'm':
            if(command=="m") {
                printMemoryDump("m 0000 65535");
            }
            else
                printMemoryDump(command);
            break;

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

void printMemoryDump(string command) {
    unsigned int baseaddr;
    int count;
    char cmd[1024]={0};
    char base[1024]={0};
    sscanf(command.c_str(),"%s %x %d",cmd,&baseaddr,&count);
//    cout<<base<<endl;
//    baseaddr=stoi(string(base), 0, 16);
    baseaddr&=0xfff0;
    unsigned int end=baseaddr+count;
    unsigned int index=baseaddr;
    printf("\n%*s\n",83,"MEMORY DUMP");
    printf("%s\n",string(158,'-').c_str());
    printf("| BASE |%27s%22s|%38s%27s|%21s%12s|\n","HEXDUMP","","DECIMAL VIEW","","ASCII VIEW","");
    printf("|      |%49s|%65s|%33s|\n"," 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F "," 000 001 002 003 004 005 006 007 008 009 00A 00B 00C 00D 00E 00F ","");
    printf("%s\n",string(158,'-').c_str());
    bool skippable=count>400;
    bool skip=false;
    while(index<=end) {
        bool nonZerofound = false;
        if(!skip)
            printf("| %04X |", index);
        for (int i = 0; i < 16; ++i) {
            if (emulator.Ram.mem[index + i] != 0) {
                nonZerofound = true;
            }
            if (!skip)
                printf(" %02X", emulator.Ram.mem[index + i]);
        }
        if (!skip)
            printf(" |");
        for (int i = 0; i < 16; ++i) {
            if (!skip)
                printf(" %03d", emulator.Ram.mem[index + i]);
        }
        if (!skip)
            printf(" |");
        for (int i = 0; i < 16; ++i) {
            char c = emulator.Ram.mem[index + i];
            char d = isprint(c) ? c : '.';
            if (!skip)
                printf(" %c", d);
        }
        if (!skip)
            printf(" |\n");
        if(skippable) {
            if (nonZerofound) {
                if (skip) {
                    skip = false;
                    index &= 0xfff0;
                } else {
                    index += 16;
                }
            } else {
                if (!skip)
                    printf("| .... |%49s|%65s|%33s|\n", "", "", "");

                skip = true;
                index += 16;
            }
        }
        else
            index+=16;

    }
    printf("%s\n",string(158,'-').c_str());


}

void printRegisters() {
    Byte a=emulator.A.get();
    Byte b=emulator.B.get();
    Byte c=emulator.C.get();
    Byte d=emulator.D.get();
    Byte e=emulator.E.get();
    Byte h=emulator.H.get();
    Byte l=emulator.L.get();
    Word bc=emulator.BC.get();
    Word de=emulator.DE.get();
    Word hl=emulator.HL.get();
    Word sp=emulator.SP.get();
    Word pc=emulator.PC.get();
    const char* zf=(emulator.flags.isSet_ZeroFlag()?"HIGH":"LOW");
    const char* cf=(emulator.flags.isSet_CarryFlag()?"HIGH":"LOW");
    const char* sf=(emulator.flags.isSet_SignFlag()?"HIGH":"LOW");
    const char* af=(emulator.flags.isSet_AuxCarryFlag()?"HIGH":"LOW");
    const char* pf=(emulator.flags.isSet_PairityFlag()?"HIGH":"LOW");
    printf("\t REGISTERS:-\n");
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | ZeroFlag      : %s\n","A",a,a,a,zf);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | CarryFlag     : %s\n","B",b,b,b,cf);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | SignFlag      : %s\n","C",c,c,c,sf);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | AuxCarryFlag  : %s\n","D",d,d,d,af);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | ParityFlag    : %s\n","E",e,e,e,pf);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","H",h,h,h);
    printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n","L",l,l,l);
    printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n","BC",bc,bc,emulator.Ram.mem[bc]);
    printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n","DE",de,de,emulator.Ram.mem[de]);
    printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n","HL",hl,hl,emulator.Ram.mem[hl]);
    printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n","SP",sp,sp,emulator.Ram.mem[sp]);
    printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n","PC",pc,pc,emulator.Ram.mem[pc]);
}

void printDebugStatus() {
    DisassembleFile();
    printRegisters();
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
