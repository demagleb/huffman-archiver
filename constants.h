//
// Created by demag on 06.10.2021.
//
#pragma once

#include "huffman_tree.h"

namespace huffman_coding {

const size_t MAX_BUFFER_SIZE = 8 * 1024;
const size_t MAX_HUFFMAN_CODE_SIZE = 259;
const size_t MAX_ALPHABET_SIZE = 259;
const size_t CHAR9_SIZE = 9;
const Char9 FILENAME_END = Char9(256);
const Char9 ONE_MORE_FILE = Char9(257);
const Char9 ARCHIVE_END = Char9(258);
}  // namespace huffman_coding
