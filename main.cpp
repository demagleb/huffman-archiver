#include <iostream>
#include <filesystem>
#include "encoder.h"
#include "decoder.h"

const std::string HELP =
    "* archiver -c archive_path file1 [file2 ...] -- archive files file1, file2, ...\n"
    "* archiver -d archive_path -- unarchive files form archive_path\n"
    "* archiver -h -- show help";
const std::string TO_MANY_ARGS = "Too many arguments";
const std::string NO_INPUT = "No input file";
const std::string NO_FILE = "No such file or directory";
const std::string BAD_FORMAT = "File has wrong format";

void Decode(std::string file) {
    std::filesystem::path path = file;
    if (!std::filesystem::exists(path)) {
        std::cout << path << std::endl;
        std::cout << NO_FILE << std::endl;
        return;
    }
    std::ifstream in(path, std::ios::binary);
    try {
        huffman_coding::Decoder decoder(in);
        bool has_file = true;
        while (has_file) {
            decoder.DecodeCodes();
            std::string filename = decoder.DecodeFilename();
            std::ofstream out(filename, std::ios::binary);
            has_file = decoder.DecodeData(out);
        }
    } catch (std::invalid_argument const&) {
        std::cout << BAD_FORMAT << std::endl;
    } catch (std::ios_base::failure const&) {
        std::cout << BAD_FORMAT << std::endl;
    }
}

void Encode(std::string path, std::vector<std::string> files) {
    std::filesystem::path archive_path(path);
    std::ofstream out(archive_path, std::ios::binary);
    huffman_coding::Encoder encoder(out);
    for (const auto& file_name : files) {
        std::filesystem::path file(file_name);
        if (!std::filesystem::exists(file)) {
            std::cout << file << std::endl;
            std::cout << NO_FILE << std::endl;
            return;
        }
        std::ifstream in(file, std::ios::binary);
        encoder.Add(in, file.filename().string());
    }
    encoder.Close();
}

int main(int argc, const char** argv) {
    try {
        if (argc == 1 || std::string(argv[1]) == "-h") {
            std::cout << HELP << std::endl;
            return 0;
        }
        if (std::string(argv[1]) == "-d") {
            if (argc > 3) {
                std::cout << TO_MANY_ARGS << std::endl;
                return 0;
            }
            if (argc < 3) {
                std::cout << NO_INPUT << std::endl;
            }
            Decode(argv[2]);
            return 0;
        }
        if (std::string(argv[1]) == "-c") {
            if (argc < 3) {
                std::cout << NO_INPUT << std::endl;
            }
            std::string archive_path = argv[2];
            std::vector<std::string> files;
            for (int i = 3; i < argc; ++i) {
                files.push_back(argv[i]);
            }
            Encode(archive_path, files);
            return 0;
        }
        std::cout << HELP << std::endl;
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}