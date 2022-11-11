#include "assembler.h"

#include <iostream>

int main() {
    Assembler a;
    std::string instr = "ADDI x1, x2, 10";
    std::string encoded_instr;
    encoded_instr = a.encode_binary(instr);
    std::cout << encoded_instr << std::endl;
    encoded_instr = a.encode_hex(instr);
    std::cout << encoded_instr << std::endl;
    return 0;
}
