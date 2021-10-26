//
// Created by demag on 03.10.2021.
//
#pragma once

#include <unordered_map>
#include <vector>
#include <bitset>
#include <memory>

namespace huffman_coding {

using Char9 = std::bitset<9>;
using HuffmanCode = std::vector<bool>;

class HuffmanTree {
public:
    HuffmanTree() : root_(std::make_shared<Node>()) {
    }
    HuffmanTree(Char9 ch);
    struct CharCodeSize {
        Char9 ch;
        size_t len;
        bool operator<(const CharCodeSize& b);
    };
    static HuffmanTree Merge(HuffmanTree& a, HuffmanTree& b);
    std::vector<CharCodeSize> GetCodeSizeTable();
    std::unordered_map<Char9, HuffmanCode> GetCodeTable(std::vector<CharCodeSize>& v);

private:
    struct Node {
        std::shared_ptr<Node> l = nullptr;
        std::shared_ptr<Node> r = nullptr;
        Char9 value;
    };

private:
    std::shared_ptr<Node> root_;
    void Dfs(std::shared_ptr<HuffmanTree::Node> v, size_t len, std::vector<CharCodeSize>& vec);
};
}  // namespace huffman_coding
