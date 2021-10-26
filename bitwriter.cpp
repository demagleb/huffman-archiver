//
// Created by demag on 03.10.2021.
//
#include "iobits.h"
#include "iobits_constants.h"

void BitWriter::Write(const std::vector<bool>& bits) {
    buffer_.insert(buffer_.end(), bits.begin(), bits.end());
    if (buffer_.size() > WRITER_MAX_BUFFER_SIZE) {
        size_t start = 0;
        std::basic_string<unsigned char> chars;
        while (start + CHAR_BIT_LENGTH < buffer_.size()) {
            unsigned char c = 0;
            for (size_t i = start; i < start + CHAR_BIT_LENGTH; ++i) {
                if (buffer_[i]) {
                    c += 1 << (i - start);
                }
            }
            chars.push_back(c);
            start += CHAR_BIT_LENGTH;
        }
        out_.write(reinterpret_cast<const char*>(chars.c_str()), chars.size());
        out_.flush();
        buffer_.erase(buffer_.begin(), buffer_.begin() + start);
    }
}

void BitWriter::Close() {
    if (!buffer_.empty()) {
        std::basic_string<unsigned char> chars;
        size_t start = 0;
        while (start < buffer_.size()) {
            unsigned char c = 0;
            for (size_t i = start; i < std::min(start + CHAR_BIT_LENGTH, buffer_.size()); ++i) {
                if (buffer_[i]) {
                    c += 1 << (i - start);
                }
            }
            chars.push_back(c);
            start += CHAR_BIT_LENGTH;
        }
        out_.write(reinterpret_cast<const char*>(chars.c_str()), chars.size());
        out_.flush();
        buffer_.clear();
    }
}