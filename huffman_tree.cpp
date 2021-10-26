//
// Created by demag on 03.10.2021.
//
#include "constants.h"
#include "huffman_tree.h"
#include "priority_queue.h"
#include <algorithm>

std::vector<huffman_coding::HuffmanTree::CharCodeSize> huffman_coding::HuffmanTree::GetCodeSizeTable() {
    std::vector<CharCodeSize> codes;
    Dfs(root_, 0, codes);
    std::sort(codes.begin(), codes.end());
    return codes;
}

void huffman_coding::HuffmanTree::Dfs(std::shared_ptr<HuffmanTree::Node> v, size_t len,
                                      std::vector<CharCodeSize>& vec) {
    if (v == nullptr) {
        return;
    }
    if (v->l == nullptr && v->r == nullptr) {
        vec.push_back({v->value, len});
    }
    Dfs(v->l, len + 1, vec);
    Dfs(v->r, len + 1, vec);
}

std::unordered_map<huffman_coding::Char9, std::vector<bool>> huffman_coding::HuffmanTree::GetCodeTable(
    std::vector<CharCodeSize>& v) {
    std::unordered_map<Char9, std::vector<bool>> result;
    std::bitset<MAX_HUFFMAN_CODE_SIZE> cur = 0;
    size_t cur_size = 0;
    for (const auto& item : v) {
        cur = cur << (item.len - cur_size);
        HuffmanCode code;
        for (size_t i = 0; i < cur.size(); ++i) {
            code.push_back(cur[i]);
        }
        while (code.size() != item.len) {
            code.pop_back();
        }
        std::reverse(code.begin(), code.end());
        result[item.ch] = code;
        for (size_t i = 0; i < cur.size(); ++i) {
            if (cur[i] == 0) {
                cur[i] = 1;
                break;
            }
            cur[i] = 0;
        }
        cur_size = item.len;
    }
    return result;
}
huffman_coding::HuffmanTree huffman_coding::HuffmanTree::Merge(huffman_coding::HuffmanTree& a,
                                                               huffman_coding::HuffmanTree& b) {
    auto result = huffman_coding::HuffmanTree();
    result.root_->l = a.root_;
    result.root_->r = b.root_;
    return result;
}
huffman_coding::HuffmanTree::HuffmanTree(huffman_coding::Char9 ch) {
    root_ = std::make_shared<Node>();
    root_->value = ch;
}

bool huffman_coding::HuffmanTree::CharCodeSize::operator<(const CharCodeSize& b) {
    if (len != b.len) {
        return len < b.len;
    }
    for (size_t i = 0; i < ch.size(); ++i) {
        if (ch[i] != b.ch[i]) {
            return ch[i] < b.ch[i];
        }
    }
    return false;
}
