//
// Created by Дмитрий Калугин on 08.10.2024.
//

#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "IBitStream.h"
#include "Bitset.h"
#include "Trie.h"

std::unordered_map<int16_t, int64_t> CalcFreq(const std::string& file_name);

std::unordered_map<int16_t, int16_t> GetCodesLen(const std::unordered_map<int16_t, int64_t>& char_freq);

std::unordered_map<int16_t, Bitset> CodesLenToCanonicalCodes(const std::unordered_map<int16_t, int16_t>& codes_len);

int16_t ReadCode(IBitStream& is, const Trie& trie);

#endif  // CPP_HSE_UTILS_H
