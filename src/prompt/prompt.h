#include <iostream>

#include "./../assembler/assembler.h"

class Prompt {
   private:
    Assembler a;

    void handle_run();
    void handle_main();
    std::string get_instr();
    void handle_instr(std::string instr);

   public:
    void run() {
        handle_run();
    };
};

void Prompt::handle_run() {
    std::cout << "RISC-V 32-bit Base Integer Instruction Set (RV32I) Assembler:" << std::endl;
    std::cout << "Enter assembly instructions or 'exit' to exit." << std::endl
              << std::endl;
    handle_main();
    std::cout << std::endl
              << "Thanks for using this tool!" << std::endl;
}

void Prompt::handle_main() {
    std::string input;
    while (true) {
        input = get_instr();
        if (input == "exit") return;
        try {
            handle_instr(input);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        std::cout << std::endl;
    }
}

std::string Prompt::get_instr() {
    std::string instr;
    // std::cin.ignore();
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, instr);
        if (instr.size() > 0) break;
        std::cout << "Error: Empty instruction" << std::endl
                  << std::endl;
    }
    return instr;
}

void Prompt::handle_instr(std::string instr) {
    EncodedInstr e = a.encode(instr);
    std::cout << std::endl;
    std::cout << "Assembly: " << e.assembly << std::endl;
    std::cout << "Format  : " << e.format << std::endl;
    std::cout << "Binary  : " << e.bin << std::endl;
    std::cout << "Hex     : " << e.hex << std::endl;
}
