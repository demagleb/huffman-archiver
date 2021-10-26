//
// Created by demag on 06.10.2021.
//
#include "priority_queue.h"

namespace huffman_coding {

size_t GetFather(size_t pos) {
    return (pos - 1) / 2;
}

size_t GetLeftSon(size_t pos) {
    return pos * 2 + 1;
}
size_t GetRightSon(size_t pos) {
    return pos * 2 + 2;
}

void PriorityQueue::Push(HuffmanTree x, size_t weight) {
    heap_.push_back({weight, x});
    size_t index = heap_.size() - 1;
    while (index > 0) {
        size_t father = GetFather(index);
        if (heap_[father].first <= heap_[index].first) {
            break;
        }
        std::swap(heap_[father], heap_[index]);
        index = father;
    }
}
std::pair<size_t, HuffmanTree> PriorityQueue::Pop() {
    auto res = heap_.front();
    heap_.front() = heap_.back();
    size_t index = 0;
    while (GetLeftSon(index) < heap_.size()) {
        size_t left_son = GetLeftSon(index);
        size_t right_son = GetRightSon(index);
        size_t min_son = left_son;
        if (right_son < heap_.size() && heap_[right_son].first < heap_[left_son].first) {
            min_son = right_son;
        }
        if (heap_[min_son].first >= heap_[index].first) {
            break;
        }
        std::swap(heap_[index], heap_[min_son]);
        index = min_son;
    }
    heap_.pop_back();
    return res;
}
size_t PriorityQueue::Size() {
    return heap_.size();
}

std::pair<size_t, HuffmanTree> PriorityQueue::Top() {
    return heap_[0];
}
}  // namespace huffman_coding
