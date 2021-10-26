//
// Created by demag on 04.10.2021.
//

#include <map>
#include "decoder.h"
#include <exception>

void huffman_coding::Decoder::DecodeCodes() {
    size_t alphabet_size = 0;
    {
        std::vector<bool> alphabet_size_bits = reader_.Read(CHAR9_SIZE);
        for (size_t i = 0; i < CHAR9_SIZE; ++i) {
            if (alphabet_size_bits[i]) {
                alphabet_size += 1 << i;
            }
        }
    }
    if (alphabet_size > MAX_ALPHABET_SIZE) {
        throw std::invalid_argument("Invalid file");
    }
    std::vector<Char9> alphabet;
    {
        for (size_t i = 0; i < alphabet_size; ++i) {
            Char9 ch;
            auto ch_bits = reader_.Read(ch.size());
            for (size_t j = 0; j < CHAR9_SIZE; ++j) {
                ch[j] = ch_bits[j];
            }
            alphabet.push_back(ch);
        }
    }
    std::vector<HuffmanTree::CharCodeSize> sizes;
    {
        size_t size = 1;
        while (alphabet_size != sizes.size()) {
            size_t cnt = 0;
            auto v = reader_.Read(CHAR9_SIZE);
            for (size_t i = 0; i < CHAR9_SIZE; ++i) {
                cnt += v[i] * (1 << i);
            }
            while (cnt > 0 && sizes.size() != alphabet_size) {
                sizes.push_back({alphabet[sizes.size()], size});
                cnt--;
            }
            ++size;
            if (cnt != 0) {
                throw std::invalid_argument("Invalid file");
            }
        }
    }
    auto codes = HuffmanTree().GetCodeTable(sizes);
    code_to_char_.clear();
    for (const auto& item : codes) {
        code_to_char_[item.second] = item.first;
    }
}

huffman_coding::Char9 huffman_coding::Decoder::GetChar() {
    std::vector<bool> bits;
    while (true) {
        while (!code_to_char_.count(bits)) {
            bits.push_back(reader_.Read(1)[0]);
            if (bits.size() > MAX_ALPHABET_SIZE) {
                throw std::invalid_argument("Invalid file");
            }
        }
        return code_to_char_.at(bits);
    }
}

std::string huffman_coding::Decoder::DecodeFilename() {
    Char9 cur_char;
    std::string filename;
    while (cur_char != FILENAME_END) {
        cur_char = GetChar();
        if (cur_char != FILENAME_END) {
            filename.push_back(static_cast<char>(cur_char.to_ulong()));
        }
    }
    return filename;
}
bool huffman_coding::Decoder::DecodeData(std::ostream& out) {
    std::string buffer;
    Char9 cur_char;
    while (cur_char != ARCHIVE_END && cur_char != ONE_MORE_FILE) {
        cur_char = GetChar();
        if (cur_char != ARCHIVE_END && cur_char != ONE_MORE_FILE) {
            char ch = static_cast<char>(cur_char.to_ulong());
            buffer.push_back(ch);
        }
        if (buffer.size() > MAX_BUFFER_SIZE) {
            out.write(buffer.c_str(), buffer.size());
            out.flush();
            buffer.clear();
        }
    }
    out.write(buffer.c_str(), buffer.size());
    out.flush();
    return cur_char == ONE_MORE_FILE;
}

bool huffman_coding::Decoder::VectorBoolCompare::operator()(const std::vector<bool>& a,
                                                            const std::vector<bool>& b) const {
    if (a.size() != b.size()) {
        return a.size() < b.size();
    }
    for (size_t i = a.size() - 1; i < a.size(); --i) {
        if (a[i] != b[i]) {
            return a[i] < b[i];
        }
    }
    return false;
}
