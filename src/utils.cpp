//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include "utils.h"

std::unordered_map<int16_t, int64_t> CalcFreq(const std::string& file_name) {
    IBitStream is(file_name);
    std::unordered_map<int16_t, int64_t> freq;
    for (unsigned char i : file_name) {
        ++freq[i];
    }
    freq[FilenameEnd] = 1;
    freq[OneMoreFile] = 1;
    freq[ArchiveEnd] = 1;
    int64_t len_read = static_cast<int64_t>(file_name.size()) + 3;
    while (!is.IsEof() and len_read + 1 > 0) {
        ++freq[is.GetByte()];
        ++len_read;
    }
    return freq;
}

std::unordered_map<int16_t, int16_t> GetCodesLen(const std::unordered_map<int16_t, int64_t>& char_freq) {
    Trie trie(char_freq);
    return trie.DfsTrie();
}

std::unordered_map<int16_t, Bitset> CodesLenToCanonicalCodes(const std::unordered_map<int16_t, int16_t>& codes_len) {
    std::vector<std::pair<int16_t, int16_t>> codes_len_sorted;
    for (auto& i : codes_len) {
        codes_len_sorted.push_back(std::make_pair(i.second, i.first));
    }
    std::sort(codes_len_sorted.begin(), codes_len_sorted.end(), std::greater<std::pair<int16_t, int16_t>>());
    std::unordered_map<int16_t, Bitset> char_codes;
    Bitset code(0);
    int16_t last_len = 0;
    {
        std::pair<int16_t, int16_t> code_len = codes_len_sorted.back();
        codes_len_sorted.pop_back();
        code.Resize(code_len.first);
        char_codes[code_len.second] = code;
        last_len = code_len.first;
    }
    while (!codes_len_sorted.empty()) {
        std::pair<int16_t, int16_t> code_len = codes_len_sorted.back();
        codes_len_sorted.pop_back();
        code.Resize(code_len.first);
        code = (++code) << static_cast<size_t>(code_len.first - last_len);
        char_codes[code_len.second] = code.Reversed();
        last_len = code_len.first;
    }
    return char_codes;
}

int16_t ReadCode(IBitStream& is, const Trie& trie) {
    std::shared_ptr<Node> v = trie.GetRoot();
    while (!v->is_terminate) {
        if (is.GetBit()) {
            if (v->right == nullptr) {
                throw std::runtime_error("ReadCode error: read code incorrect");
            }
            v = v->right;
        } else {
            if (v->left == nullptr) {
                throw std::runtime_error("ReadCode error: read code incorrect");
            }
            v = v->left;
        }
    }
    if (!v->is_terminate) {
        throw std::runtime_error("ReadCode error: read code incorrect");
    }
    return v->c;
}