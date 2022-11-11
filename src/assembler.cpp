#include "assembler.h"

#include <iostream>

int main() {
    Assembler a;
    std::string instr;
    std::string encoded_instr;
    instr = "ADDI x1, x2, 10";
    encoded_instr = a.encode_binary(instr);
    std::cout << encoded_instr << std::endl;
    instr = "Addi X1, x2, 10";
    encoded_instr = a.encode_hex(instr);
    std::cout << encoded_instr << std::endl;
    return 0;
}
