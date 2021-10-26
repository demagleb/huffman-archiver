//
// Created by demag on 03.10.2021.
//
#pragma once

#include "constants.h"
#include <fstream>
#include <vector>
#include <deque>

class BitWriter {
public:
    BitWriter(std::ostream& out) : out_(out) {
    }
    void Write(const std::vector<bool>& bits);
    void Close();

private:
    std::ostream& out_;
    std::deque<bool> buffer_;
};

class BitReader {
public:
    BitReader(std::istream& in) : in_(in) {
    }
    std::vector<bool> Read(size_t n);

private:
    std::istream& in_;
    std::deque<bool> buffer_;
};