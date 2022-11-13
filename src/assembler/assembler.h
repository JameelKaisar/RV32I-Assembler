#include <bitset>
// #include <format> // Yaar yath gaseh C++20 aasun
#include <regex>
#include <string>
#include <vector>

#include "./../helper/helper.h"
#include "./../rv32i/rv32i.h"

class Assembler {
   private:
    EncodedInstr encode_instr(std::string instr);
    std::vector<std::string> parse_instr(std::string instr);
    std::vector<std::string> parse_instr_format(std::string instr, InstrFormat format);
    InstrParams parse_tokens(std::vector<std::string> instr_tokens);
    std::string parse_imm(std::string imm, InstrFormat format);
    std::string parse_params(InstrParams instr_params);
    ImmParts parse_imm_parts(std::string imm);

   public:
    EncodedInstr encode(std::string instr) {
        return encode_instr(instr);
    }
};

EncodedInstr Assembler::encode_instr(std::string instr) {
    std::vector<std::string> instr_tokens = parse_instr(instr);
    if (instr_tokens.size() == 0) {
        throw std::runtime_error("Invalid instruction");
    }
    std::string instr_assembly = instr_tokens[instr_tokens.size() - 1];
    InstrParams instr_params = parse_tokens(instr_tokens);
    std::string encoded_instr_bin = parse_params(instr_params);
    std::string encoded_instr_hex = "0x" + bin_to_hex(encoded_instr_bin);
    std::string format(1, instr_info[instr_params.name].format);
    return {instr_assembly, encoded_instr_bin, encoded_instr_hex, format};
}

std::vector<std::string> Assembler::parse_instr(std::string instr) {
    std::vector<std::string> instr_tokens;
    std::regex name_regex("\\s*([[:alpha:]]+)\\s+.*");
    std::smatch matches;
    if (!std::regex_match(instr, matches, name_regex)) {
        return instr_tokens;
    }
    std::string instr_name = matches.str(1);
    for_each(instr_name.begin(), instr_name.end(), [](char& c) {
        c = tolower(c);
    });
    if (instr_info.find(instr_name) == instr_info.end()) {
        return instr_tokens;
    }
    InstrFormat format = instr_info[instr_name].format;
    instr_tokens = parse_instr_format(instr, format);
    for (int i = 0; i < instr_tokens.size(); i++) {
        for_each(instr_tokens[i].begin(), instr_tokens[i].end(), [](char& c) {
            c = tolower(c);
        });
        if (i != 0 && instr_tokens[i][0] == 'x' && instr_registers.find(instr_tokens[i]) == instr_registers.end()) {
            return std::vector<std::string>();
        }
    }
    return instr_tokens;
}

std::vector<std::string> Assembler::parse_instr_format(std::string instr, InstrFormat format) {
    std::vector<std::string> instr_tokens;
    std::string instr_assembly;
    std::regex instr_regex;
    std::smatch matches;
    switch (format) {
        case InstrR:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*([xX][0-9]+)\\s*,\\s*([xX][0-9]+)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}, {}", matches.str(1), matches.str(2), matches.str(3), matches.str(4));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3) + ", " + matches.str(4);
                instr_tokens = {matches.str(1), matches.str(2), matches.str(3), matches.str(4), instr_assembly};
            }
            break;
        case InstrI:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}, {}", matches.str(1), matches.str(2), matches.str(3), matches.str(4));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3) + ", " + matches.str(4);
                instr_tokens = {matches.str(1), matches.str(2), matches.str(3), matches.str(4), instr_assembly};
            }
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*\\(\\s*([xX][0-9]+)\\s*\\)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}({})", matches.str(1), matches.str(2), matches.str(3), matches.str(4));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3) + "(" + matches.str(4) + ")";
                instr_tokens = {matches.str(1), matches.str(2), matches.str(4), matches.str(3), instr_assembly};
            }
            break;
        case InstrS:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*\\(\\s*([xX][0-9]+)\\s*\\)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}({})", matches.str(1), matches.str(2), matches.str(3), matches.str(4));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3) + "(" + matches.str(4) + ")";
                instr_tokens = {matches.str(1), matches.str(2), matches.str(4), matches.str(3), instr_assembly};
            }
            break;
        case InstrB:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}, {}", matches.str(1), matches.str(2), matches.str(3), matches.str(4));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3) + ", " + matches.str(4);
                instr_tokens = {matches.str(1), matches.str(2), matches.str(3), matches.str(4), instr_assembly};
            }
            break;
        case InstrU:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}", matches.str(1), matches.str(2), matches.str(3));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3);
                instr_tokens = {matches.str(1), matches.str(2), matches.str(3), instr_assembly};
            }
            break;
        case InstrJ:
            instr_regex = std::regex("\\s*([[:alpha:]]+)\\s+([xX][0-9]+)\\s*,\\s*(-?[0-9]+)\\s*");
            if (std::regex_match(instr, matches, instr_regex)) {
                // instr_assembly = std::format("{} {}, {}", matches.str(1), matches.str(2), matches.str(3));
                instr_assembly = matches.str(1) + " " + matches.str(2) + ", " + matches.str(3);
                instr_tokens = {matches.str(1), matches.str(2), matches.str(3), instr_assembly};
            }
            break;
    }
    return instr_tokens;
}

InstrParams Assembler::parse_tokens(std::vector<std::string> instr_tokens) {
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
            imm = parse_imm(instr_tokens[3], format);
            instr_params = {name, rd, rs1, rs2, imm};
            break;
        case InstrS:
            rs2 = instr_registers[instr_tokens[1]];
            rs1 = instr_registers[instr_tokens[2]];
            imm = parse_imm(instr_tokens[3], format);
            instr_params = {name, rd, rs1, rs2, imm};
            break;
        case InstrB:
            rs1 = instr_registers[instr_tokens[1]];
            rs2 = instr_registers[instr_tokens[2]];
            imm = parse_imm(instr_tokens[3], format);
            instr_params = {name, rd, rs1, rs2, imm};
            break;
        case InstrU:
            rd = instr_registers[instr_tokens[1]];
            imm = parse_imm(instr_tokens[2], format);
            instr_params = {name, rd, rs1, rs2, imm};
            break;
        case InstrJ:
            rd = instr_registers[instr_tokens[1]];
            imm = parse_imm(instr_tokens[2], format);
            instr_params = {name, rd, rs1, rs2, imm};
            break;
    }
    return instr_params;
}

std::string Assembler::parse_imm(std::string imm, InstrFormat format) {
    int imm_int = std::stoi(imm);
    std::string imm_bin;
    std::string imm_bin_32;
    switch (format) {
        case InstrI:
            imm_bin = std::bitset<12>(imm_int).to_string();
            imm_bin_32 = std::string(20, imm_bin[0]) + imm_bin;
            break;
        case InstrS:
            imm_bin = std::bitset<12>(imm_int).to_string();
            imm_bin_32 = std::string(20, imm_bin[0]) + imm_bin;
            break;
        case InstrB:
            imm_int >>= 1;
            imm_bin = std::bitset<12>(imm_int).to_string();
            imm_bin_32 = std::string(19, imm_bin[0]) + imm_bin + "0";
            break;
        case InstrU:
            // NOTE: imm_int chun right shift karun kehe
            imm_bin = std::bitset<20>(imm_int).to_string();
            imm_bin_32 = imm_bin + std::string(12, '0');
            break;
        case InstrJ:
            imm_int >>= 1;
            imm_bin = std::bitset<20>(imm_int).to_string();
            imm_bin_32 = std::string(11, imm_bin[0]) + imm_bin + "0";
            break;
    }
    return imm_bin_32;
}

std::string Assembler::parse_params(InstrParams instr_params) {
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

ImmParts Assembler::parse_imm_parts(std::string imm) {
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
