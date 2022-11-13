#include "assembler.h"

#include <iostream>

int main() {
    Assembler a;
    std::vector<std::string> instr;
    instr.push_back("ADDI x1, x2, 10");                       // 0x00a10093
    instr.push_back("AddI X1, x2, -10");                      // 0xff610093
    instr.push_back("   sw   x1   ,   -10   (   x2   )   ");  // 0xfe112b23
    instr.push_back("lw x1,-10(x2)");                         // 0xff612083
    instr.push_back("lui x1, 10");                            // 0x0000a0b7
    instr.push_back("auipc X31, -10");                        // 0xffff6f97
    instr.push_back("JALR X1, X2, 10");                       // 0x00a100e7
    instr.push_back("jal x1, -10");                           // 0xff7ff0ef
    instr.push_back("BLTU x1, x2, -8");                       // 0xfe20ece3
    instr.push_back("BGEU x1, x2, 4");                        // 0x0020f263
    instr.push_back("SLTIU x2, x3, 5");                       // 0x0051b113

    for (auto x : instr) {
        EncodedInstr e = a.encode(x);
        std::cout << "Assembly: " << e.assembly << std::endl;
        std::cout << "Format  : " << e.format << std::endl;
        std::cout << "Binary  : " << e.bin << std::endl;
        std::cout << "Hex     : " << e.hex << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
