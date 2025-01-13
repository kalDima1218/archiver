//
// Created by Дмитрий Калугин on 11.10.2024.
//

#ifndef CPP_HSE_BITSET_H
#define CPP_HSE_BITSET_H

#include <stdexcept>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include "consts.h"

class Bitset {
private:
    std::vector<int64_t> bits_;
    size_t size_;

    void ClearUnusedBits();

public:
    Bitset();
    explicit Bitset(size_t n);
    Bitset(int64_t x, size_t n);

    Bitset& operator=(const Bitset& other);

    size_t Size() const;

    void Resize(size_t new_size);

    void Set(size_t pos, bool value);

    bool operator[](size_t pos) const;

    Bitset operator<<(size_t shift) const;
    Bitset operator>>(size_t shift) const;

    Bitset& operator++();

    bool operator<(const Bitset& other) const;
    bool operator>(const Bitset& other) const;

    explicit operator char() const;
    explicit operator int16_t() const;
    explicit operator int32_t() const;
    explicit operator int64_t() const;

    Bitset Reversed() const;

    std::string ToString() const;
};

#endif  // CPP_HSE_BITSET_H
