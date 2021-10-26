//
// Created by demag on 06.10.2021.
//
#pragma once

#include "huffman_tree.h"

namespace huffman_coding {

class PriorityQueue {
public:
    void Push(HuffmanTree x, size_t weight);
    std::pair<size_t, HuffmanTree> Pop();
    std::pair<size_t, HuffmanTree> Top();
    size_t Size();

private:
    std::vector<std::pair<size_t, HuffmanTree>> heap_;
};
}  // namespace huffman_coding
