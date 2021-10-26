//
// Created by demag on 03.10.2021.
//
#include <catch.hpp>
#include <random>
#include <set>
#include "huffman_tree.h"
#include "encoder.h"
#include "decoder.h"
#include "priority_queue.h"

std::random_device rd;
std::mt19937 engine(rd());
std::uniform_int_distribution<size_t> distribution;

const size_t N_ITER_COUNT = 1'000'000;
const size_t N2_ITER_COUNT = 500;
const size_t N3_ITER_COUNT = 100;

TEST_CASE("HEAP") {
    huffman_coding::PriorityQueue queue;
    std::multiset<size_t> set;
    for (size_t i = 0; i < N_ITER_COUNT; ++i) {
        bool add = distribution(engine) % 2;
        if (add || queue.Size() == 0) {
            size_t num = distribution(engine);
            queue.Push(huffman_coding::HuffmanTree(), num);
            set.insert(num);
        } else {
            auto value = queue.Pop();
            REQUIRE(value.first == *set.begin());
            set.erase(set.begin());
        }
    }
}

TEST_CASE("BitReader, BitWriter") {
    for (size_t j = 0; j < N2_ITER_COUNT; ++j) {
        std::vector<bool> bits;
        for (size_t i = 0; i < N2_ITER_COUNT; ++i) {
            bits.push_back(static_cast<char>(distribution(engine)));
        }
        std::ostringstream out;
        BitWriter writer(out);
        for (const auto& item : bits) {
            writer.Write(std::vector<bool>{item});
        }
        writer.Close();
        std::vector<bool> out_bits;
        std::istringstream in(out.str());
        BitReader reader(in);
        for (size_t i = 0; i < N2_ITER_COUNT; ++i) {
            std::vector<bool> bit = reader.Read(1);
            out_bits.push_back(bit[0]);
        }
        for (size_t i = 0; i < bits.size(); ++i) {
            REQUIRE(bits[i] == out_bits[i]);
        }
    }
    std::istringstream in;
    BitReader reader(in);
    bool exception = false;
    try {
        reader.Read(1);
    } catch (const std::ios_base::failure&) {
        exception = true;
    }
    REQUIRE(exception);
}

TEST_CASE("HuffmanTree") {
    for (size_t j = 0; j < N2_ITER_COUNT; ++j) {
        std::unordered_map<huffman_coding::Char9, size_t> frequency;
        for (size_t i = 0; i < N2_ITER_COUNT; ++i) {
            ++frequency[huffman_coding::Char9(distribution(engine))];
        }
        huffman_coding::HuffmanTree tree = huffman_coding::Encoder::BuildHuffmanTree(frequency);
        auto size_table = tree.GetCodeSizeTable();
        auto code_table = tree.GetCodeTable(size_table);
        for (const auto& code1 : code_table) {
            for (const auto& code2 : code_table) {
                if (code1.first == code2.first || code1.second.size() > code2.second.size()) {
                    continue;
                }
                bool prefix = true;
                for (size_t i = 0; i < code1.second.size(); ++i) {
                    prefix &= code1.second[i] == code2.second[i];
                }
                REQUIRE(!prefix);
            }
        }
    }
}

TEST_CASE("Encoder, Decoder 1 file") {
    std::vector<std::string> inputs;
    for (size_t i = 0; i < N2_ITER_COUNT; ++i) {
        std::string str;
        for (size_t j = 0; j < N2_ITER_COUNT; ++j) {
            str.push_back(static_cast<char>(distribution(engine)));
        }
        inputs.push_back(str);
    }
    for (const auto& input : inputs) {
        std::ostringstream encoder_out;
        std::istringstream encoder_in(input);
        huffman_coding::Encoder encoder(encoder_out);
        encoder.Add(encoder_in, "asdf");
        encoder.Close();
        std::istringstream decoder_in(encoder_out.str());
        huffman_coding::Decoder decoder(decoder_in);
        decoder.DecodeCodes();
        REQUIRE(decoder.DecodeFilename() == "asdf");
        std::ostringstream decoder_out;
        decoder.DecodeData(decoder_out);
        REQUIRE(decoder_out.str() == input);
    }
}
TEST_CASE("Encoder, Decoder 2 files") {
    std::vector<std::string> inputs;
    for (size_t i = 0; i < N3_ITER_COUNT; ++i) {
        std::string str;
        for (size_t j = 0; j < N3_ITER_COUNT; ++j) {
            str.push_back(static_cast<char>(distribution(engine)));
        }
        inputs.push_back(str);
    }
    for (const auto& input1 : inputs) {
        for (const auto& input2 : inputs) {
            std::ostringstream encoder_out;
            std::istringstream encoder_in1(input1);
            std::istringstream encoder_in2(input2);
            huffman_coding::Encoder encoder(encoder_out);
            encoder.Add(encoder_in1, "asdf");
            encoder.Add(encoder_in2, "asdfasdf");
            encoder.Close();
            std::istringstream decoder_in(encoder_out.str());
            huffman_coding::Decoder decoder(decoder_in);

            decoder.DecodeCodes();
            REQUIRE(decoder.DecodeFilename() == "asdf");
            std::ostringstream decoder_out1;
            decoder.DecodeData(decoder_out1);
            REQUIRE(decoder_out1.str() == input1);

            decoder.DecodeCodes();
            REQUIRE(decoder.DecodeFilename() == "asdfasdf");
            std::ostringstream decoder_out2;
            decoder.DecodeData(decoder_out2);
            REQUIRE(decoder_out2.str() == input2);
        }
    }
}

TEST_CASE("Decoder invalid file") {
    std::istringstream in("invalid file");
    huffman_coding::Decoder decoder(in);
    bool exception = false;
    try {
        decoder.DecodeCodes();
    } catch (const std::exception&) {
        exception = true;
    }
    REQUIRE(exception);
}