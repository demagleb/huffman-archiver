//
// Created by demag on 04.10.2021.
//
#pragma once

#include "huffman_tree.h"
#include <fstream>
#include "iobits.h"
#include <map>
#include "constants.h"

namespace huffman_coding {

class Decoder {
public:
    Decoder(std::istream& in) : reader_(in) {
    }
    void DecodeCodes();
    bool DecodeData(std::ostream& out);
    std::string DecodeFilename();

private:
    struct VectorBoolCompare {
        bool operator()(const std::vector<bool>& a, const std::vector<bool>& b) const;
    };
    Char9 GetChar();
    BitReader reader_;
    std::map<std::vector<bool>, Char9, VectorBoolCompare> code_to_char_;
};
}  // namespace huffman_coding
