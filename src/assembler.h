#include <bitset>
#include <regex>
#include <string>
#include <vector>

#include "helper.h"
#include "rv32i.h"

class Assembler {
   private:
    std::string encode(std::string instr) {
        std::vector<std::string> instr_tokens = parse_instr(instr);
        InstrParams instr_params = parse_tokens(instr_tokens);
        std::string encoded_instr = parse_params(instr_params);
        return encoded_instr;
    }

    std::vector<std::string> parse_instr(std::string instr) {
        std::regex re(R"([\s|,]+)");
        std::sregex_token_iterator first{instr.begin(), instr.end(), re, -1}, last;
        std::vector<std::string> tokens(first, last);
        tokens.erase(remove_if(tokens.begin(), tokens.end(), [](std::string const& s) { return s.size() == 0; }), tokens.end());
        for (int i = 0; i < tokens.size(); i++) {
            for_each(tokens[i].begin(), tokens[i].end(), [i](char& c) {
                c = (i == 0) ? toupper(c) : tolower(c);
            });
        }
        return tokens;
    }

    InstrParams parse_tokens(std::vector<std::string> instr_tokens) {
        std::string name = instr_tokens[0];
        std::string rd;
        std::string rs1;
        std::string rs2;
        std::string imm;
        InstrFormat format = instr_info[name].format;
        InstrParams instr_params;
        switch (format) {
            case InstrR:
                rd = instr_registers[instr_tokens[1]];
                rs1 = instr_registers[instr_tokens[2]];
                rs2 = instr_registers[instr_tokens[3]];
                instr_params = {name, rd, rs1, rs2, imm};
                break;
            case InstrI:
                rd = instr_registers[instr_tokens[1]];
                rs1 = instr_registers[instr_tokens[2]];
                imm = parse_imm(instr_tokens[3]);
                instr_params = {name, rd, rs1, rs2, imm};
                break;
            case InstrS:
                rs1 = instr_registers[instr_tokens[1]];
                rs2 = instr_registers[instr_tokens[2]];
                imm = parse_imm(instr_tokens[3]);
                instr_params = {name, rd, rs1, rs2, imm};
                break;
            case InstrB:
                rs1 = instr_registers[instr_tokens[1]];
                rs2 = instr_registers[instr_tokens[2]];
                imm = parse_imm(instr_tokens[3]);
                instr_params = {name, rd, rs1, rs2, imm};
                break;
            case InstrU:
                rd = instr_registers[instr_tokens[1]];
                imm = parse_imm(instr_tokens[2]);
                instr_params = {name, rd, rs1, rs2, imm};
                break;
            case InstrJ:
                rd = instr_registers[instr_tokens[1]];
                imm = parse_imm(instr_tokens[2]);
                instr_params = {name, rd, rs1, rs2, imm};
                break;
        }
        return instr_params;
    }

    std::string parse_imm(std::string imm) {
        return std::bitset<32>(std::stoi(imm)).to_string();
    }

    std::string parse_params(InstrParams instr_params) {
        std::string name = instr_params.name;
        std::string rs1 = instr_params.rs1;
        std::string rs2 = instr_params.rs2;
        std::string rd = instr_params.rd;
        std::string imm = instr_params.imm;
        InstrFormat format = instr_info[name].format;
        std::string opcode = instr_info[name].opcode;
        std::string funct3 = instr_info[name].funct3;
        std::string funct7 = instr_info[name].funct7;
        std::string encoded_instr;
        ImmParts imm_parts;
        switch (format) {
            case InstrR:
                encoded_instr = funct7 + rs2 + rs1 + funct3 + rd + opcode;
                break;
            case InstrI:
                imm_parts = parse_imm_parts(imm);
                if (funct7.size() > 0) {
                    encoded_instr = funct7 + imm_parts.imm_4_0 + rs1 + funct3 + rd + opcode;
                } else {
                    encoded_instr = imm_parts.imm_11_0 + rs1 + funct3 + rd + opcode;
                }
                break;
            case InstrS:
                imm_parts = parse_imm_parts(imm);
                encoded_instr = imm_parts.imm_11_5 + rs2 + rs1 + funct3 + imm_parts.imm_4_0 + opcode;
                break;
            case InstrB:
                imm_parts = parse_imm_parts(imm);
                encoded_instr = imm_parts.imm_12 + imm_parts.imm_10_5 + rs2 + rs1 + funct3 + imm_parts.imm_4_1 + imm_parts.imm_11 + opcode;
                break;
            case InstrU:
                imm_parts = parse_imm_parts(imm);
                encoded_instr = imm_parts.imm_31_12 + rd + opcode;
                break;
            case InstrJ:
                imm_parts = parse_imm_parts(imm);
                encoded_instr = imm_parts.imm_20 + imm_parts.imm_10_1 + imm_parts.imm_11 + imm_parts.imm_19_12 + rd + opcode;
                break;
        }
        return encoded_instr;
    }

    ImmParts parse_imm_parts(std::string imm) {
        ImmParts imm_parts;
        imm_parts.imm = imm;
        imm_parts.imm_11_0 = imm.substr(31 - (11), (11 - 0) + 1);
        imm_parts.imm_11_5 = imm.substr(31 - (11), (11 - 5) + 1);
        imm_parts.imm_4_0 = imm.substr(31 - (4), (4 - 0) + 1);
        imm_parts.imm_12 = imm.substr(31 - (12), (12 - 12) + 1);
        imm_parts.imm_10_5 = imm.substr(31 - (10), (10 - 5) + 1);
        imm_parts.imm_4_1 = imm.substr(31 - (4), (4 - 1) + 1);
        imm_parts.imm_11 = imm.substr(31 - (11), (11 - 11) + 1);
        imm_parts.imm_31_12 = imm.substr(31 - (31), (31 - 12) + 1);
        imm_parts.imm_20 = imm.substr(31 - (20), (20 - 20) + 1);
        imm_parts.imm_10_1 = imm.substr(31 - (10), (10 - 1) + 1);
        imm_parts.imm_19_12 = imm.substr(31 - (19), (19 - 12) + 1);
        return imm_parts;
    }

   public:
    std::string encode_binary(std::string instr) {
        return encode(instr);
    }

    std::string encode_hex(std::string instr) {
        return "0x" + bin_to_hex(encode(instr));
    }
};
