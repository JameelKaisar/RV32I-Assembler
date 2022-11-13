#include <string>

#include "./../enums/enums.h"

typedef struct InstrParams {
    std::string name;
    std::string rd;
    std::string rs1;
    std::string rs2;
    std::string imm;
} InstrParams;

typedef struct InstrInfo {
    InstrFormat format;
    std::string opcode;
    std::string funct3;
    std::string funct7;
} InstrInfo;

typedef struct ImmParts {
    std::string imm;
    std::string imm_11_0;
    std::string imm_11_5;
    std::string imm_4_0;
    std::string imm_12;
    std::string imm_10_5;
    std::string imm_4_1;
    std::string imm_11;
    std::string imm_31_12;
    std::string imm_20;
    std::string imm_10_1;
    std::string imm_19_12;
} ImmParts;

typedef struct EncodedInstr {
    std::string assembly;
    std::string bin;
    std::string hex;
    std::string format;
} EncodedInstr;
