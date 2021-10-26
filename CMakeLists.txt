project(huffman-archiver)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

include_directories(util)


add_executable(archiver
        main.cpp huffman_tree.cpp huffman_tree.h encoder.cpp encoder.h iobits.h bitwriter.cpp bitreader.cpp decoder.cpp decoder.h priority_queue.h priority_queue.cpp constants.h iobits_constants.h)

add_catch(test_archiver
        test.cpp
        huffman_tree.cpp
        encoder.cpp
        decoder.cpp
        bitreader.cpp
        bitwriter.cpp
        priority_queue.cpp)
