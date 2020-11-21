#include <iostream>
#include "Assembler/Assembler.h"
#include "Core/Emulator.h"
using namespace std;
int main() {
    Assembler a;
    a.Assemble_file("did.asm");
    emulator emu;
    emu.reset();
    emu.load_binary(a.assembled);
    emu.run();
}
