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
    for (auto x : instr) {
        std::cout << a.encode_binary(x) << std::endl;
        std::cout << a.encode_hex(x) << std::endl;
    }
    return 0;
}
