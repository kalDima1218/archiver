//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include "Trie.h"

Node::Node() : left(nullptr), right(nullptr), is_terminate(false) {
}

Node::Node(int16_t c) : left(nullptr), right(nullptr), c(c), is_terminate(true) {
}

Node::Node(int16_t c, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right)
    : left(left), right(right), c(c), is_terminate(false) {
}

void Trie::DfsTrie(std::unordered_map<int16_t, int16_t>& code_len, int16_t cur_len,
                   std::shared_ptr<Node> cur_node) const {
    if (cur_node->left != nullptr) {
        DfsTrie(code_len, static_cast<int16_t>(cur_len + 1), cur_node->left);
    }
    if (cur_node->right != nullptr) {
        DfsTrie(code_len, static_cast<int16_t>(cur_len + 1), cur_node->right);
    }
    if (cur_node->is_terminate) {
        code_len[cur_node->c] = cur_len;
    }
}

Trie::Trie() : root_(nullptr) {
}

Trie::Trie(const std::unordered_map<int16_t, int64_t>& char_freq) {
    Heap<std::pair<int16_t, std::shared_ptr<Node>>,
         decltype([](const std::pair<int16_t, std::shared_ptr<Node>>& a,
                     const std::pair<int16_t, std::shared_ptr<Node>>& b) -> bool {
             if (a.first < b.first) {
                 return true;
             } else if (a.first > b.first) {
                 return false;
             } else {
                 return a.second->c < b.second->c;
             }
         })>
        chars_freq_ordered;
    for (auto [chr, freq] : char_freq) {
        chars_freq_ordered.Push(std::make_pair(freq, std::make_shared<Node>(chr)));
    }
    while (chars_freq_ordered.Size() > 1) {
        std::pair<int16_t, std::shared_ptr<Node>> left = chars_freq_ordered.Top();
        chars_freq_ordered.Pop();
        std::pair<int16_t, std::shared_ptr<Node>> right = chars_freq_ordered.Top();
        chars_freq_ordered.Pop();
        chars_freq_ordered.Push(std::make_pair(
            left.first + right.first,
            std::make_shared<Node>(std::min(left.second->c, right.second->c), left.second, right.second)));
    }
    root_ = chars_freq_ordered.Top().second;
}

std::shared_ptr<Node> Trie::GetRoot() const {
    return root_;
}

void Trie::AddNode(const Bitset& code, int16_t c) {
    if (root_ == nullptr) {
        root_ = std::make_shared<Node>();
    }
    std::shared_ptr<Node> v = root_;
    for (int32_t i = 0; i < code.Size(); ++i) {
        if (v->is_terminate) {
            throw std::runtime_error("Trie::AddNode error: code incorrect");
        }
        if (code[i]) {
            if (v->right == nullptr) {
                v->right = std::make_shared<Node>();
            }
            v = v->right;
        } else {
            if (v->left == nullptr) {
                v->left = std::make_shared<Node>();
            }
            v = v->left;
        }
    }
    if (v->is_terminate) {
        throw std::runtime_error("Trie::AddNode error: code already exists");
    }
    v->c = c;
    v->is_terminate = true;
}

std::unordered_map<int16_t, int16_t> Trie::DfsTrie() const {
    std::unordered_map<int16_t, int16_t> code_len;
    DfsTrie(code_len, 0, root_);
    return code_len;
}