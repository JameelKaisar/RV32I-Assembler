#include <string>

std::string bin_to_hex(std::string bin) {
    std::string hex;
    for (int i = 0; i < 32; i += 4) {
        std::string bin_nibble = bin.substr(i, 4);
        if (bin_nibble == "0000") {
            hex += "0";
        } else if (bin_nibble == "0001") {
            hex += "1";
        } else if (bin_nibble == "0010") {
            hex += "2";
        } else if (bin_nibble == "0011") {
            hex += "3";
        } else if (bin_nibble == "0100") {
            hex += "4";
        } else if (bin_nibble == "0101") {
            hex += "5";
        } else if (bin_nibble == "0110") {
            hex += "6";
        } else if (bin_nibble == "0111") {
            hex += "7";
        } else if (bin_nibble == "1000") {
            hex += "8";
        } else if (bin_nibble == "1001") {
            hex += "9";
        } else if (bin_nibble == "1010") {
            hex += "a";
        } else if (bin_nibble == "1011") {
            hex += "b";
        } else if (bin_nibble == "1100") {
            hex += "c";
        } else if (bin_nibble == "1101") {
            hex += "d";
        } else if (bin_nibble == "1110") {
            hex += "e";
        } else if (bin_nibble == "1111") {
            hex += "f";
        }
    }
    return hex;
}
