//
// Created by demag on 03.10.2021.
//
#pragma once

#include "huffman_tree.h"
#include <fstream>
#include <filesystem>
#include "iobits.h"
#include "constants.h"

namespace huffman_coding {

class Encoder {
public:
    Encoder(std::ostream& out) : writer_(out) {
    }
    void Add(std::istream& in, const std::string& filename);
    void Close();
    static HuffmanTree BuildHuffmanTree(const std::unordered_map<Char9, size_t>& frequency);

private:
    void WriteCodeSizeTable(const std::vector<HuffmanTree::CharCodeSize>& table);
    void WriteFilename(const std::string& filename, const std::unordered_map<Char9, HuffmanCode>& codes);
    void WriteData(std::istream& in, const std::unordered_map<Char9, HuffmanCode>& codes);
    BitWriter writer_;
    HuffmanCode archive_end_code_;
    HuffmanCode one_more_file_code_;
};
}  // namespace huffman_coding