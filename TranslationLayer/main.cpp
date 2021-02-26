#include "../Core/Emulator.h"
#include "../Helper/Argparse.h"
#include "../Helper/Disassembler.h"
#include "../Assembler/Assembler.h"
#include "../Assembler/ExecFileFormat/Execfile.h"
#include "../Helper/Screen.h"

void jsonMemory();
void jsonbreakpoints();

void jsonPorts();

void jsonRegisters();

void DisassembleFile();

void execute(string &command);

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
namespace config {
    string filename;
    string prompt;
    bool run;
    bool disassemble;
    bool jsonOutput;
    bool write;
    bool isValidExecutable;
    bool running = true;
}

int main(int argc, char **argv) {
    Argparse parser("A commandline Emulator for your 8085 assembly code");
    parser.AddArgument("f", "filename", Argparse::Path, "Path of the file that is to be loaded", true, true);
    parser.AddArgument("d", "disassemble", Argparse::Boolean, "Only print the disassembly of the file");
    parser.AddArgument("p", "prompt", Argparse::String, "Set the prompt to the given string");
    parser.AddArgument("j", "json", Argparse::Boolean, "Dump all outputs as json");
    parser.ParseArgument(argc, argv);
    config::disassemble = parser.getArg("-d").found;
    config::filename = parser.getArg("f").Value;
    config::prompt = parser.getArg("-p").found ? parser.getArg("-p").Value : "PRS>> ";
    config::isValidExecutable = Execfile::isExecutable(config::filename);
    config::jsonOutput = parser.getArg("-j").found;
    emulator.reset();
    if (config::isValidExecutable) {
        execfile = Execfile(config::filename);
        disassembler = Disassembler(execfile.Binarycode);
        if (!execfile.header.Stripped)
            disassembler.loadLabelMap(execfile.labelmap);
        disassembler.Disassemble();
        emulator.load_binary(execfile.Binarycode);
    } else {
        assembler.Assemble_file(config::filename);
        if (!assembler.parser.errorlist.empty()) {
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
        disassembler = Disassembler(assembler.assembled);
        disassembler.loadLabelMap(assembler.label2addr);
        disassembler.Disassemble();
    }
    if (config::disassemble) {
        DisassembleFile();
        Screen::flush();
        exit(0);
    }
    while (config::running) {
        char cmd[1024] = {0};
        cout << config::prompt;
        cin.getline(cmd, 1024);
        string command(cmd);
        execute(command);
        Screen::flush();
    }
}

void execute(string &command) {
    switch (tolower(command[0])) {
        case 'd':
            DisassembleFile();
            break;
        case 'q':
            config::running = false;
            exit(0);
        case 'm':
            if (command == "m") {
                if (config::jsonOutput)
                    jsonMemory();
                else
                    printMemoryDump("m 0000 65535");
            } else
                printMemoryDump(command);
            break;
        case 's':
            print50stack();
            break;
        case 'c':
            if (command.size() > 1 && tolower(command[1]) == 'l') {
                if (system("CLS")) system("clear");
            } else {
                emulator.run();
                if (!(command == "c;")) printDebugStatus();
            }
            break;
        case 'p':
            if (config::jsonOutput)
                jsonPorts();
            else if (command == "p") {
                dumpPorts("ALL");
            } else {
                dumpPorts(command);
            }
            break;
        case 'n':
            emulator.single_step();
            if (!(command == "n;")) printDebugStatus();
            break;
        case 'r':
            if (command == string("reset")) {
                emulator.reset();
            } else {
                if (config::jsonOutput)
                    jsonRegisters();
                else
                    printRegisters();
            }
            break;
        case 'w':
            switch (tolower(command[1])) {
                case 'm':
                    setMemory(command);
                    break;
                case 'p':
                    setPorts(command);
                    break;
                case '?':
                    Screen::printf("\t%-30s\t\t%s\n", "wm ADDRESS VALUE1 VALUE2",
                                   "Writes into memory the VALUE1 VALUE2... starting from ADDRESS");
                    Screen::printf("\t%-30s\t\t%s\n", "wp ADDRESS VALUE1 VALUE2",
                                   "Writes into Ports the VALUE1 VALUE2... starting from ADDRESS");
                    Screen::printf("\t%-30s\t\t%s\n", "w?", "Shows this help menu");


            }
        case 'b': {
            switch (tolower(command[1])) {
                case 'c':
                    emulator.breakpoints.clear();
                    break;
                case 'l':
                    if(config::jsonOutput)
                        jsonbreakpoints();
                    else{
                        for (auto item:emulator.breakpoints) {
                            Screen::printf("\tBreakpoint at %04XH \n", item & 0xffff);
                        }
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
                    Screen::printf("\t%-30s\t\t%s\n", "ba ADDRESS", "Adds new breakpoint at specified address");
                    Screen::printf("\t%-30s\t\t%s\n", "bc", "Clears all the breakpoints.");
                    Screen::printf("\t%-30s\t\t%s\n", "bd ADDRESS", "Deletes all the breakpoints at specified address");
                    Screen::printf("\t%-30s\t\t%s\n", "bl", "list all the breakpoints");
                    Screen::printf("\t%-30s\t\t%s\n", "b?", "Displays this help ");

                }
                    break;
            }
            break;
        }
        case '?': {
            Screen::printf("\t%-30s\t\t%s\n", "b?", "Help for Breakpoints related commands");
            Screen::printf("\t%-30s\t\t%s\n", "c", "Continue/Runs the program till program halts or breakpoint is hit");
            Screen::printf("\t%-30s\t\t%s\n", "c;", "Continue/Run the program silently (NO OUTPUT)");
            Screen::printf("\t%-30s\t\t%s\n", "cl", "clears the screen");
            Screen::printf("\t%-30s\t\t%s\n", "d", "prints disassembly of the current program");
            Screen::printf("\t%-30s\t\t%s\n", "m", "Dumps all the memory in concise form");
            Screen::printf("\t%-30s\t\t%s\n", "m BEGIN_ADDRESS BYTE_COUNT",
                           "Dumps BYTE_COUNT no of bytes starting from BEGIN_ADDRESS");
            Screen::printf("\t%-30s\t\t%s\n", "ms ADDRESS1 ADDRESS2 ...", "Displays content of memory locations ");
            Screen::printf("\t%-30s\t\t%s\n", "n", "Executes next instruction");
            Screen::printf("\t%-30s\t\t%s\n", "n;", "Executes next instruction silently (NO OUTPUT)");
            Screen::printf("\t%-30s\t\t%s\n", "p", "Dumps all the ports");
            Screen::printf("\t%-30s\t\t%s\n", "p ADDRESS", "Prints the value of port address ADDRESS");
            Screen::printf("\t%-30s\t\t%s\n", "r", "Prints the Information on Registers");
            Screen::printf("\t%-30s\t\t%s\n", "s", "Dump up to 20 Values Around stack As Words(16bit)");
            Screen::printf("\t%-30s\t\t%s\n", "w?", "Help for commands related to Writing into Memory,port,etc");
            Screen::printf("\t%-30s\t\t%s\n", "reset",
                           "resets the Emulator ie for restarting program(brkpnt not lost)");
            Screen::printf("\t%-30s\t\t%s\n", "?", "Displays this help ");
        }
    }
}

void print50stack() {
    int sp = emulator.SP.get();
    Screen::printf("\t\t\t\tStack View\n");
    if (sp == 0xffff) {
        Screen::printf("\t\tStack is Empty!\n");
        return;
    }
    Word index = sp;
    for (int i = 0; i < 50 && index < 0xffff; ++i) {
        const char *pointer = (index == sp) ? "SP=>" : "";
        Byte vl = emulator.Ram.mem[index++];
        Byte vh = emulator.Ram.mem[index++];
        Word bc = (vh << 8) + vl;
        Screen::printf("\t\t%5s:+%03d~| %04X | %04X(Hex) %05d(Dec)  Points to=> %02X\n", pointer, i * 2, index, bc, bc,
                       emulator.Ram.mem[bc]);
    }
}

void dumpPorts(string command) {
    if (command == "ALL") {
        int index = 0;
        Screen::printf("\n%*s\n", 83, " PORTS DUMP");
        Screen::printf("%s\n", string(161, '-').c_str());
        Screen::printf("|   BASE  |%27s%22s|%38s%27s|%21s%12s|\n", "HEXDUMP", "", "DECIMAL VIEW", "", "ASCII VIEW", "");
        Screen::printf("|         |%49s|%65s|%33s|\n", " 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ",
                       " 000 001 002 003 004 005 006 007 008 009 00A 00B 00C 00D 00E 00F ", "");
        Screen::printf("%s\n", string(161, '-').c_str());
        while (index <= 255) {
            Screen::printf("| port-%02X |", index);
            for (int i = 0; i < 16; ++i) {
                if (index + i > 255)break;
                Screen::printf(" %02X", emulator.ports.place[index + i]);
            }
            Screen::printf(" |");
            for (int i = 0; i < 16; ++i) {
                if (index + i > 255)break;
                Screen::printf(" %03d", emulator.ports.place[index + i]);
            }
            Screen::printf(" |");
            for (int i = 0; i < 16; ++i) {
                if (index + i > 255)break;
                char c = emulator.ports.place[index + i];
                char d = isprint(c) ? c : '.';
                Screen::printf(" %c", d);
            }
            Screen::printf(" |\n");
            index += 16;
        }
        Screen::printf("%s\n", string(161, '-').c_str());
    } else {
        stringstream ss(command);
        string token;
        ss >> token;
        while (!ss.eof()) {
            ss >> token;
            Byte address = stoi(token, 0, 16);
            Byte value = emulator.ports.place[address];
            Screen::printf("\t\tPORT-%02X : %02X(Hex) %03d(Dec) '%c'(Ascii)\n", address, value, value, value);
        }
    }
}


void setPorts(string &command) {
    stringstream ss(command);
    string token;
    ss >> token;
    ss >> token;
    Word start = stoi(token, 0, 16);
    if (start > 255)return;
    Word index = start;
    while (!ss.eof()) {
        ss >> token;
        Byte value;
        if (regex_match(token, regex("^[0-9]+$")))
            value = stoi(token);
        else if (regex_match(token, regex("^[0-9a-fA-F]+[hH]$")))
            value = stoi(token, 0, 16);
        else if (regex_match(token, regex("^[0-1]+[bB]$")))
            value = stoi(token, 0, 2);
        else if (regex_match(token, regex("^[0-7]+[oO]$")))
            value = stoi(token, 0, 8);
        else if (regex_match(token, regex("'.'")))
            value = token[1];
        else break;
        emulator.ports.place[index] = value;
        index += 1;
    }
}

void setMemory(string command) {
    stringstream ss(command);
    string token;
    ss >> token;
    ss >> token;
    Word start = stoi(token, 0, 16);
    Word index = start;
    while (!ss.eof()) {
        ss >> token;
        Byte value;
        if (regex_match(token, regex("^[0-9]+$")))
            value = stoi(token);
        else if (regex_match(token, regex("^[0-9a-fA-F]+[hH]$")))
            value = stoi(token, 0, 16);
        else if (regex_match(token, regex("^[0-1]+[bB]$")))
            value = stoi(token, 0, 2);
        else if (regex_match(token, regex("^[0-7]+[oO]$")))
            value = stoi(token, 0, 8);
        else if (regex_match(token, regex("'.'")))
            value = token[1];
        else break;
        emulator.Ram.mem[index] = value;
        index += 1;
    }
}

void printMemoryDump(string command) {
    unsigned int baseaddr;
    int count;
    char cmd[1024] = {0};
    char base[1024] = {0};
    sscanf(command.c_str(), "%s %x %d", cmd, &baseaddr, &count);
    if (tolower(cmd[1]) == 's') {
        stringstream ss(command);
        string token;
        ss >> token;
        while (!ss.eof()) {
            ss >> token;
            Word address = stoi(token, 0, 16);
            Byte value = emulator.Ram.mem[address];
            Screen::printf("\t\tMemory Address-%04X : %02X(Hex) %03d(Dec) '%c'(Ascii)\n", address, value, value, value);
        }
        return;
    }
    baseaddr &= 0xfff0U;
    unsigned int end = baseaddr + count;
    unsigned int index = baseaddr;
    Screen::printf("\n%*s\n", 83, "MEMORY DUMP");
    Screen::printf("%s\n", string(158, '-').c_str());
    Screen::printf("| BASE |%27s%22s|%38s%27s|%21s%12s|\n", "HEXDUMP", "", "DECIMAL VIEW", "", "ASCII VIEW", "");
    Screen::printf("|      |%49s|%65s|%33s|\n", " 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ",
                   " 000 001 002 003 004 005 006 007 008 009 00A 00B 00C 00D 00E 00F ", "");
    Screen::printf("%s\n", string(158, '-').c_str());
    bool skippable = count > 400;
    bool skip = false;
    while (index <= end) {
        bool nonZerofound = false;
        if (!skip)
            Screen::printf("| %04X |", index);
        for (int i = 0; i < 16; ++i) {
            if (emulator.Ram.mem[index + i] != 0) {
                nonZerofound = true;
            }
            if (!skip)
                Screen::printf(" %02X", emulator.Ram.mem[index + i]);
        }

        if (!skip) {
            Screen::printf(" |");
            for (int i = 0; i < 16; ++i) {
                Screen::printf(" %03d", emulator.Ram.mem[index + i]);
            }
            Screen::printf(" |");
            for (int i = 0; i < 16; ++i) {
                char c = emulator.Ram.mem[index + i];
                char d = isprint(c) ? c : '.';
                Screen::printf(" %c", d);
            }

            Screen::printf(" |\n");
        }
        if (skippable) {
            if (nonZerofound) {
                if (skip) {
                    skip = false;
                    index &= 0xfff0;
                } else {
                    index += 16;
                }
            } else {
                if (!skip)
                    Screen::printf("| .... |%49s|%65s|%33s|\n", "", "", "");

                skip = true;
                index += 16;
            }
        } else
            index += 16;

    }
    Screen::printf("%s\n", string(158, '-').c_str());


}

void jsonPorts() {
    bool skip = false;
    bool firstprop = true;
    Screen::printf("{");
    for (int index = 0; index < 0xff;) {
        bool nonZerofound = false;
        if (!skip) {
            if (!firstprop)Screen::printf(",\n");
            Screen::printf("\"%d\":[", index);
            firstprop = false;
        }
        bool firstelem = true;
        for (int i = 0; i < 16; ++i) {
            if (emulator.ports.place[index + i] != 0) {
                nonZerofound = true;
            }
            if (!skip) {
                if (!firstelem)Screen::printf(",");
                Screen::printf("%d", emulator.ports.place[index + i]);
                firstelem = false;
            }
        }
        if (!skip)
            Screen::printf("]");
        if (nonZerofound) {
            if (skip) {
                skip = false;
                index &= 0xfff0;
            } else {
                index += 16;
            }
        } else {
            skip = true;
            index += 16;
        }
    }
    Screen::printf("}\n");
}

void jsonMemory() {
    bool skip = false;
    bool firstprop = true;
    Screen::printf("{");
    for (int index = 0; index < 0xffff;) {
        bool nonZerofound = false;
        if (!skip) {
            if (!firstprop)Screen::printf(",\n");
            Screen::printf("\"%d\":[", index);
            firstprop = false;
        }
        bool firstelem = true;
        for (int i = 0; i < 16; ++i) {
            if (emulator.Ram.mem[index + i] != 0) {
                nonZerofound = true;
            }
            if (!skip) {
                if (!firstelem)Screen::printf(",");
                Screen::printf("%d", emulator.Ram.mem[index + i]);
                firstelem = false;
            }
        }
        if (!skip)
            Screen::printf("]");
        if (nonZerofound) {
            if (skip) {
                skip = false;
                index &= 0xfff0;
            } else {
                index += 16;
            }
        } else {
            skip = true;
            index += 16;
        }
    }
    Screen::printf("}\n");
}

void jsonRegisters() {
    Byte a = emulator.A.get();
    Byte b = emulator.B.get();
    Byte c = emulator.C.get();
    Byte d = emulator.D.get();
    Byte e = emulator.E.get();
    Byte h = emulator.H.get();
    Byte l = emulator.L.get();
    Word sp = emulator.SP.get();
    Word pc = emulator.PC.get();
    Byte flags = emulator.flags.get();
    Screen::printf("[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", a, b, c, d, e, h, l, sp, pc, flags);
}
void jsonbreakpoints(){
        Screen::printf("[");
        bool first=true;
        for(auto brkpt : emulator.breakpoints)
        {
            if(!first)Screen::printf(",");
            first=false;
            Screen::printf("%d",brkpt);
        }
        Screen::printf("]");

    }

void printRegisters() {
    Byte a = emulator.A.get();
    Byte b = emulator.B.get();
    Byte c = emulator.C.get();
    Byte d = emulator.D.get();
    Byte e = emulator.E.get();
    Byte h = emulator.H.get();
    Byte l = emulator.L.get();
    Word bc = emulator.BC.get();
    Word de = emulator.DE.get();
    Word hl = emulator.HL.get();
    Word sp = emulator.SP.get();
    Word pc = emulator.PC.get();
    const char *zf = (emulator.flags.isSet_ZeroFlag() ? "HIGH" : "LOW");
    const char *cf = (emulator.flags.isSet_CarryFlag() ? "HIGH" : "LOW");
    const char *sf = (emulator.flags.isSet_SignFlag() ? "HIGH" : "LOW");
    const char *af = (emulator.flags.isSet_AuxCarryFlag() ? "HIGH" : "LOW");
    const char *pf = (emulator.flags.isSet_PairityFlag() ? "HIGH" : "LOW");
    Screen::printf("\t REGISTERS:-\n");
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | ZeroFlag      : %s\n", "A", a, a,
                   isprint(a) ? a : ' ', zf);
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | CarryFlag     : %s\n", "B", b, b,
                   isprint(b) ? b : ' ', cf);
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | SignFlag      : %s\n", "C", c, c,
                   isprint(c) ? c : ' ', sf);
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | AuxCarryFlag  : %s\n", "D", d, d,
                   isprint(d) ? d : ' ', af);
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)            | ParityFlag    : %s\n", "E", e, e,
                   isprint(e) ? e : ' ', pf);
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n", "H", h, h, isprint(h) ? h : ' ');
    Screen::printf("\t\t%-5s : %02X(Hex) %03d(Dec) '%c'(Ascii)\n", "L", l, l, isprint(l) ? l : ' ');
    Screen::printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n", "BC", bc, bc, emulator.Ram.mem[bc]);
    Screen::printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n", "DE", de, de, emulator.Ram.mem[de]);
    Screen::printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n", "HL", hl, hl, emulator.Ram.mem[hl]);
    Screen::printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n", "SP", sp, sp, emulator.Ram.mem[sp]);
    Screen::printf("\t\t%-5s : %04X(Hex) %05d(Dec)  Points to=> %02X\n", "PC", pc, pc, emulator.Ram.mem[pc]);
}

void printDebugStatus() {
    Screen::printf("\n\t\t\tDisassembly View:-\n\n");
    DisassembleFile();
    Screen::printf("\n\n%s\n\n", string(158, '-').c_str());
    printRegisters();
    Screen::printf("\n\n%s\n\n", string(158, '-').c_str());
    if (emulator.halted)Screen::printf("\t\t\tEmulater Has Halted!\n\t\t\tPress reset to restart from Beginning\n\n");
    if (emulator.brkhit)
        Screen::printf(
                "\t\t\tBreakpoint Hit!\n\t\t\tinspect memory(m) registers(r) ports(p) and stack(s) then continue(c/n)\n\n");
}

void DisassembleFile() {
    if (!emulator.halted && disassembler.line_no(emulator.PC.get()) == 0) {
        auto lmap = disassembler.labelMap;
        disassembler = Disassembler(emulator.binary_from_pc(), emulator.PC.get());
        disassembler.labelMap = lmap;
        disassembler.Disassemble();
    }
    for (auto &item:disassembler.Disassembled) {
        char bytes[20] = {0};
        for (int i = 0; i < item.size; ++i) {
            sprintf(bytes, "%s %02X", bytes, item.rawBytes[i] & 0xff);
        }
        char brk = (emulator.breakpoints.count(item.address) > 0) ? '*' : ' ';
        const char *pc = ((emulator.PC.get() == item.address) ? "PC=>" : "    ");
        char labelsep = (item.label.empty() ? ' ' : ':');
        if (disassembler.grapher.blockbreaks.count(item.address) > 0) {
            string arrow;
            for (auto ch: item.arrows) {
                if (ch == '|' || ch == '`' || ch == '+') arrow += '|';
                else arrow += " ";
            }
            Screen::printf("\t%20s|%11s|\n", arrow.c_str(), "");
        }
        Screen::printf("\t%20s|%s%c%4XH | %-9s %20s%c %-20s\n", item.arrows.c_str(), pc, brk, item.address, bytes,
                       item.label.c_str(), labelsep, item.Assembly.c_str());
        if (item.Assembly == "HLT") break;
    }
}
