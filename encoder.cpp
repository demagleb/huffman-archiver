//
// Created by demag on 03.10.2021.
//
#include <vector>
#include "encoder.h"
#include "priority_queue.h"

void huffman_coding::Encoder::Add(std::istream& in, const std::string& filename) {
    writer_.Write(one_more_file_code_);
    std::unordered_map<Char9, size_t> frequency;
    frequency[FILENAME_END] = 1;
    frequency[ONE_MORE_FILE] = 1;
    frequency[ARCHIVE_END] = 1;
    for (const auto& ch : filename) {
        unsigned char uch = ch;
        ++frequency[static_cast<Char9>(uch)];
    }
    while (!in.eof()) {
        unsigned char ch = in.get();
        if (!in.eof()) {
            ++frequency[static_cast<Char9>(ch)];
        }
    }
    HuffmanTree tree = BuildHuffmanTree(frequency);
    auto table = tree.GetCodeSizeTable();
    WriteCodeSizeTable(table);
    auto codes = tree.GetCodeTable(table);
    WriteFilename(filename, codes);
    in.clear();
    in.seekg(0);
    WriteData(in, codes);
    one_more_file_code_ = codes.at(Char9(ONE_MORE_FILE));
    archive_end_code_ = codes.at(Char9(ARCHIVE_END));
}
void huffman_coding::Encoder::Close() {
    writer_.Write(archive_end_code_);
    writer_.Close();
}
void huffman_coding::Encoder::WriteCodeSizeTable(const std::vector<HuffmanTree::CharCodeSize>& table) {
    {
        Char9 alphabet_size = Char9(table.size());
        std::vector<bool> alphabet_size_bits;
        for (size_t i = 0; i < alphabet_size.size(); ++i) {
            alphabet_size_bits.push_back(alphabet_size[i]);
        }
        writer_.Write(alphabet_size_bits);
    }
    {
        std::vector<bool> alphabet;
        for (const auto& item : table) {
            for (size_t i = 0; i < item.ch.size(); ++i) {
                alphabet.push_back(item.ch[i]);
            }
        }
        writer_.Write(alphabet);
    }
    {
        std::vector<size_t> symbols_count(table.back().len);
        for (const auto& item : table) {
            symbols_count[item.len - 1]++;
        }
        std::vector<bool> counts;
        for (const auto& item : symbols_count) {
            Char9 count(item);
            for (size_t i = 0; i < count.size(); ++i) {
                counts.push_back(count[i]);
            }
        }
        writer_.Write(counts);
    }
}
void huffman_coding::Encoder::WriteFilename(const std::string& filename,
                                            const std::unordered_map<Char9, HuffmanCode>& codes) {
    for (const auto& item : filename) {
        unsigned char uch = item;
        writer_.Write(codes.at(Char9(uch)));
    }
    writer_.Write(codes.at(FILENAME_END));
}
void huffman_coding::Encoder::WriteData(std::istream& in, const std::unordered_map<Char9, HuffmanCode>& codes) {
    while (!in.eof()) {
        unsigned char ch = in.get();
        if (!in.eof()) {
            writer_.Write(codes.at(Char9(ch)));
        }
    }
}
huffman_coding::HuffmanTree huffman_coding::Encoder::BuildHuffmanTree(
    const std::unordered_map<Char9, size_t>& frequency) {
    PriorityQueue queue;
    for (const auto& item : frequency) {
        queue.Push(HuffmanTree(item.first), item.second);
    }
    while (queue.Size() != 1) {
        auto a = queue.Pop();
        auto b = queue.Pop();
        queue.Push(HuffmanTree::Merge(a.second, b.second), a.first + b.first);
    }
    return queue.Top().second;
}
