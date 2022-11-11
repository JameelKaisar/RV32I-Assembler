#include "assembler.h"

#include <iostream>

int main() {
    Assembler a;
    std::string instr = "ADDI x1, x2, 10";
    std::string encoded_instr = a.encode(instr);
    std::cout << encoded_instr << std::endl;
    return 0;
}
