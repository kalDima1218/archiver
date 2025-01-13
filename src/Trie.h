//
// Created by Дмитрий Калугин on 08.10.2024.
//

#ifndef CPP_HSE_TRIE_H
#define CPP_HSE_TRIE_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Bitset.h"
#include "Heap.h"

struct Node {
    std::shared_ptr<Node> left, right;
    int16_t c;
    bool is_terminate;

    Node();

    explicit Node(const int16_t c);

    Node(int16_t c, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right);
};

class Trie {
private:
    std::shared_ptr<Node> root_;

    void DfsTrie(std::unordered_map<int16_t, int16_t>& code_len, int16_t cur_len, std::shared_ptr<Node> cur_node) const;

public:
    Trie();

    explicit Trie(const std::unordered_map<int16_t, int64_t>& char_freq);

    std::shared_ptr<Node> GetRoot() const;

    void AddNode(const Bitset& code, int16_t c);

    std::unordered_map<int16_t, int16_t> DfsTrie() const;
};

#endif  // CPP_HSE_TRIE_H
