//
// Created by demag on 04.10.2021.
//
#include "iobits.h"
#include "iobits_constants.h"

std::vector<bool> BitReader::Read(size_t n) {
    if (n > buffer_.size()) {
        while (buffer_.size() < READER_MAX_BUFFER_SIZE && !in_.eof()) {
            unsigned char c = in_.get();
            if (!in_.eof()) {
                for (size_t i = 0; i < CHAR_BIT_LENGTH; ++i) {
                    buffer_.push_back(static_cast<bool>(c & (1 << i)));
                }
            }
        }
    }
    if (n > buffer_.size()) {
        throw std::ios_base::failure("Unexpected end of file");
    }
    std::vector<bool> result(buffer_.begin(), buffer_.begin() + n);
    for (size_t i = 0; i < n; ++i) {
        buffer_.pop_front();
    }
    return result;
}