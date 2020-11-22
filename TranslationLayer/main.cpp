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

void setMemory(string command);

void setPorts(string &command);

void dumpPorts(string command);

void print50stack();

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
    parser.AddArgument("d","disassemble",Argparse::Boolean,"Only print the disassembly of the file");
    parser.AddArgument("p","prompt",Argparse::String,"Set the prompt to the given string");
    parser.ParseArgument(argc,argv);
    config::disassemble=parser.getArg("-d").found;
    config::filename=parser.getArg("f").Value;
    config::prompt=parser.getArg("-p").found?parser.getArg("-p").Value:"PRS>> ";
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
        if(!assembler.parser.errorlist.empty())
        {
            cout << "                         List of all Errors                         " << endl << endl;
            for (auto e:assembler.parser.errorlist) {
                if (e.line != string(""))
                    cerr << "Error On line " << e.line_no << " : " << e.line << endl << "\t" << e.msg << endl << endl;
                else
                    cerr << "Error On line " << e.line_no << " : " << e.msg << endl << endl;
            }
            cout << "\n\n              Please solve above errors and retry                  " << endl;
            exit(1);
        }
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
        case 's':
            print50stack();
            break;
        case 'c':
            if(command.size()>1&&tolower(command[1])=='l')
            {
                if (system("CLS")) system("clear");
            }
            else{
                emulator.run();
                if(!(command=="c;")) printDebugStatus();
            }
            break;
        case 'p':
            if(command=="p")
            {
                dumpPorts("ALL");
            }
            else
            {
                dumpPorts(command);
            }
            break;
        case 'n':
            emulator.single_step();
            if(!(command=="n;")) printDebugStatus();
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
        case 'w':
            switch(tolower(command[1]))
            {
                case 'm':
                    setMemory(command);
                    break;
                case 'p':
                    setPorts(command);
                    break;
                case '?':
                    printf("\t%-30s\t\t%s\n", "wm ADDRESS VALUE1 VALUE2", "Writes into memory the VALUE1 VALUE2... starting from ADDRESS");
                    printf("\t%-30s\t\t%s\n", "wp ADDRESS VALUE1 VALUE2", "Writes into Ports the VALUE1 VALUE2... starting from ADDRESS");
                    printf("\t%-30s\t\t%s\n", "w?", "Shows this help menu");


            }
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
                    break;
            }
        case '?': {
            printf("\t%-30s\t\t%s\n", "b?", "Help for Breakpoints related commands");
            printf("\t%-30s\t\t%s\n", "c", "Continue/Runs the program till program halts or breakpoint is hit");
            printf("\t%-30s\t\t%s\n", "c;", "Continue/Run the program silently (NO OUTPUT)");
            printf("\t%-30s\t\t%s\n", "cl", "clears the screen");
            printf("\t%-30s\t\t%s\n", "d", "prints disassembly of the current program");
            printf("\t%-30s\t\t%s\n", "m", "Dumps all the memory in concise form");
            printf("\t%-30s\t\t%s\n", "m BEGIN_ADDRESS BYTE_COUNT", "Dumps BYTE_COUNT no of bytes starting from BEGIN_ADDRESS");
            printf("\t%-30s\t\t%s\n", "ms ADDRESS1 ADDRESS2 ...", "Displays content of memory locations ");
            printf("\t%-30s\t\t%s\n", "n", "Executes next instruction");
            printf("\t%-30s\t\t%s\n", "n;", "Executes next instruction silently (NO OUTPUT)");
            printf("\t%-30s\t\t%s\n", "p", "Dumps all the ports");
            printf("\t%-30s\t\t%s\n", "p ADDRESS", "Prints the value of port address ADDRESS");
            printf("\t%-30s\t\t%s\n", "r", "Prints the Information on Registers");
            printf("\t%-30s\t\t%s\n", "s", "Dump up to 20 Values Around stack As Words(16bit)");
            printf("\t%-30s\t\t%s\n", "w?", "Help for commands related to Writing into Memory,port,etc");
            printf("\t%-30s\t\t%s\n", "reset", "resets the Emulator ie for restarting program(brkpnt not lost)");
            printf("\t%-30s\t\t%s\n", "?", "Displays this help ");
        }
    }
}

void print50stack() {
    int sp=emulator.SP.get();
    printf("\t\t\t\tStack View\n");
    if(sp==0xffff)
    {
        printf("\t\tStack is Empty!\n");
        return;
    }
    Word index=sp;
    for (int i = 0; i < 50; ++i) {
        const char*pointer=(index==sp)?"SP=>":"";
        Byte vl=emulator.Ram.mem[index++];
        Byte vh=emulator.Ram.mem[index++];
        Word bc=(vh<<8)+vl;
        printf("\t\t%5s:+%03d~| %04X | %04X(Hex) %05d(Dec)  Points to=> %02X\n",pointer,index,i*2,bc,bc,emulator.Ram.mem[bc]);
    }
}

void dumpPorts(string command) {
    if (command == "ALL") {
        int index = 0;
        printf("\n%*s\n", 83, " PORTS DUMP");
        printf("%s\n", string(158, '-').c_str());
        printf("|   BASE  |%27s%22s|%38s%27s|%21s%12s|\n", "HEXDUMP", "", "DECIMAL VIEW", "", "ASCII VIEW", "");
        printf("|         |%49s|%65s|%33s|\n", " 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ",
               " 000 001 002 003 004 005 006 007 008 009 00A 00B 00C 00D 00E 00F ", "");
        printf("%s\n", string(158, '-').c_str());
        while (index <= 255) {
            printf("| port-%02X |", index);
            for (int i = 0; i < 16; ++i) {
                if(index+i>255)break;
                printf(" %02X", emulator.ports.place[index + i]);
            }
            printf(" |");
            for (int i = 0; i < 16; ++i) {
                if(index+i>255)break;
                printf(" %03d", emulator.ports.place[index + i]);
            }
            printf(" |");
            for (int i = 0; i < 16; ++i) {
                if(index+i>255)break;
                char c = emulator.ports.place[index + i];
                char d = isprint(c) ? c : '.';
                printf(" %c", d);
            }
            printf(" |\n");
            index += 16;
        }
        printf("%s\n", string(158, '-').c_str());
    }
    else
    {
        stringstream ss(command);
        string token;
        ss>>token;
        while(!ss.eof()) {
            ss >> token;
            Byte address = stoi(token, 0, 16);
            Byte value=emulator.ports.place[address];
            printf("\t\tPORT-%02X : %02X(Hex) %03d(Dec) '%c'(Ascii)\n",address,value,value,value);
        }
    }
}


void setPorts(string &command) {
    stringstream ss(command);
    string token;
    ss>>token;
    ss>>token;
    Word start=stoi(token,0,16);
    if(start>255)return;
    Word index=start;
    while(!ss.eof())
    {
        ss>>token;
        Byte value;
        if(regex_match(token,regex("^[0-9]+$")))
            value=stoi(token);
        else if(regex_match(token,regex("^[0-9a-fA-F]+[hH]$")))
            value=stoi(token,0,16);
        else if(regex_match(token,regex("^[0-1]+[bB]$")))
            value=stoi(token,0,2);
        else if(regex_match(token,regex("^[0-7]+[oO]$")))
            value=stoi(token,0,8);
        else if(regex_match(token,regex("'.'")))
            value=token[1];
        else break;
        emulator.ports.place[index]=value;
        index+=1;
    }
}

void setMemory(string command) {
    stringstream ss(command);
    string token;
    ss>>token;
    ss>>token;
    Word start=stoi(token,0,16);
    Word index=start;
    while(!ss.eof())
    {
        ss>>token;
        Byte value;
        if(regex_match(token,regex("^[0-9]+$")))
            value=stoi(token);
        else if(regex_match(token,regex("^[0-9a-fA-F]+[hH]$")))
            value=stoi(token,0,16);
        else if(regex_match(token,regex("^[0-1]+[bB]$")))
            value=stoi(token,0,2);
        else if(regex_match(token,regex("^[0-7]+[oO]$")))
            value=stoi(token,0,8);
        else if(regex_match(token,regex("'.'")))
            value=token[1];
        else break;
        emulator.Ram.mem[index]=value;
        index+=1;
    }
}

void printMemoryDump(string command) {
    unsigned int baseaddr;
    int count;
    char cmd[1024]={0};
    char base[1024]={0};
    sscanf(command.c_str(),"%s %x %d",cmd,&baseaddr,&count);
    if(tolower(cmd[1])=='s')
    {
        stringstream ss(command);
        string token;
        ss>>token;
        while(!ss.eof()) {
            ss >> token;
            Word address = stoi(token, 0, 16);
            Byte value=emulator.Ram.mem[address];
            printf("\t\tMemory Address-%04X : %02X(Hex) %03d(Dec) '%c'(Ascii)\n",address,value,value,value);
        }
        return;
    }
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

        if (!skip) {
            printf(" |");
            for (int i = 0; i < 16; ++i) {
                    printf(" %03d", emulator.Ram.mem[index + i]);
            }
                printf(" |");
            for (int i = 0; i < 16; ++i) {
                char c = emulator.Ram.mem[index + i];
                char d = isprint(c) ? c : '.';
                    printf(" %c", d);
            }

                printf(" |\n");
        }
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
    printf("\n\t\t\tDisassembly View:-\n\n");
    DisassembleFile();
    printf("\n\n%s\n\n",string(158,'-').c_str());
    printRegisters();
    printf("\n\n%s\n\n",string(158,'-').c_str());
    if(emulator.halted)printf("\t\t\tEmulater Has Halted!\n\t\t\tPress reset to restart from Beginning\n\n");
    if(emulator.brkhit)printf("\t\t\tBreakpoint Hit!\n\t\t\tinspect memory(m) registers(r) ports(p) and stack(s) then continue(c/n)\n\n");
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
            printf("\t%20s|%s%c%4XH | %-9s %20s%c %-20s\n",item.arrows.c_str(),pc,brk,item.address,bytes,item.label.c_str(),labelsep,item.Assembly.c_str());
        }
}
