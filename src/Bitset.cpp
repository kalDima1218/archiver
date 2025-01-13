//
// Created by Дмитрий Калугин on 14.10.2024.
//

#include "Bitset.h"

void Bitset::ClearUnusedBits() {
    if (size_ == 0) {
        return;
    }
    size_t used_bits_in_last_block = size_ % Int64Len;
    if (used_bits_in_last_block != 0) {
        int64_t mask = (1LL << used_bits_in_last_block) - 1;
        bits_.back() &= mask;
    }
}

Bitset::Bitset() : bits_(), size_(0) {
}

Bitset::Bitset(size_t n) : bits_((n + Int64Len - 1) / Int64Len, 0), size_(n) {
}

Bitset::Bitset(int64_t x, size_t n) : bits_((n + Int64Len - 1) / Int64Len, 0), size_(n) {
    bits_[0] = static_cast<int64_t>(x);
    ClearUnusedBits();
}

Bitset& Bitset::operator=(const Bitset& other) {
    if (this != &other) {
        bits_ = other.bits_;
        size_ = other.size_;
    }
    return *this;
}

size_t Bitset::Size() const {
    return size_;
}

void Bitset::Resize(size_t new_size) {
    size_t new_blocks = (new_size + Int64Len - 1) / Int64Len;
    bits_.resize(new_blocks, 0);
    size_ = new_size;
    ClearUnusedBits();
}

void Bitset::Set(size_t pos, bool value) {
    if (pos >= size_) {
        throw std::runtime_error("Bitset::Set error: index out of range");
    }
    size_t block = pos / Int64Len;
    size_t offset = pos % Int64Len;
    if (value) {
        bits_[block] |= (1LL << offset);
    } else {
        bits_[block] &= ~(1LL << offset);
    }
}

bool Bitset::operator[](size_t pos) const {
    if (pos >= size_) {
        throw std::runtime_error("Bitset::operator[] error: index out of range");
    }
    size_t block = pos / Int64Len;
    size_t offset = pos % Int64Len;
    return (bits_[block] >> offset) & 1ULL;
}

Bitset Bitset::operator<<(size_t shift) const {
    if (shift == 0) {
        return *this;
    }
    Bitset result(size_);
    size_t block_shift = shift / Int64Len;
    size_t bit_shift = shift % Int64Len;
    size_t num_blocks = bits_.size();
    for (size_t i = 0; i < num_blocks; ++i) {
        size_t dest_index = i + block_shift;
        if (dest_index < result.bits_.size()) {
            result.bits_[dest_index] |= bits_[i] << bit_shift;
            if (bit_shift != 0 && dest_index + 1 < result.bits_.size()) {
                result.bits_[dest_index + 1] |= bits_[i] >> (Int64Len - bit_shift);
            }
        }
    }
    result.ClearUnusedBits();
    return result;
}

Bitset Bitset::operator>>(size_t shift) const {
    if (shift == 0) {
        return *this;
    }
    if (shift >= size_) {
        return Bitset(size_);
    }
    Bitset result(size_);
    size_t block_shift = shift / Int64Len;
    size_t bit_shift = shift % Int64Len;
    for (size_t i = 0; i + block_shift < bits_.size(); ++i) {
        int64_t upper = bits_[i + block_shift] >> bit_shift;
        int64_t lower = 0;
        if (bit_shift != 0 && (i + block_shift + 1) < bits_.size()) {
            lower = bits_[i + block_shift + 1] << (Int64Len - bit_shift);
        }
        result.bits_[i] = upper | lower;
    }
    result.ClearUnusedBits();
    return result;
}

Bitset& Bitset::operator++() {
    bool carry = true;
    for (size_t i = 0; i < bits_.size() && carry; ++i) {
        int64_t mask = (i == bits_.size() - 1 && size_ % Int64Len != 0) ? ((1LL << (size_ % Int64Len)) - 1) : ~0LL;
        bits_[i] = (bits_[i] + 1) & mask;
        carry = bits_[i] == 0;
        if (bits_[i] & ~mask) {
            bits_[i] &= mask;
        }
    }
    ClearUnusedBits();
    return *this;
}

bool Bitset::operator<(const Bitset& other) const {
    size_t max_blocks = std::max(bits_.size(), other.bits_.size());
    for (size_t i = max_blocks; i > 0; --i) {
        int64_t a = (i <= bits_.size()) ? bits_[i - 1] : 0;
        int64_t b = (i <= other.bits_.size()) ? other.bits_[i - 1] : 0;
        if (a != b) {
            return a < b;
        }
    }
    return false;
}

bool Bitset::operator>(const Bitset& other) const {
    return other < *this;
}

Bitset::operator char() const {
    if (size_ == 0) {
        return 0;
    }
    if (size_ > CharLen) {
        throw std::runtime_error("Bitset::operator char error: bitset too big to cast");
    }
    return static_cast<char>(bits_[0]);
}

Bitset::operator int16_t() const {
    if (size_ == 0) {
        return 0;
    }
    if (size_ > Int16Len) {
        throw std::runtime_error("Bitset::operator int16_t error: bitset too big to cast");
    }
    return static_cast<int16_t>(bits_[0]);
}

Bitset::operator int32_t() const {
    if (size_ == 0) {
        return 0;
    }
    if (size_ > Int32Len) {
        throw std::runtime_error("Bitset::operator int32_t error: bitset too big to cast");
    }
    return static_cast<int32_t>(bits_[0]);
}

Bitset::operator int64_t() const {
    if (size_ == 0) {
        return 0;
    }
    if (size_ > Int64Len) {
        throw std::runtime_error("Bitset::operator int64_t error: bitset too big to cast");
    }
    return bits_[0];
}

Bitset Bitset::Reversed() const {
    Bitset result = *this;
    result.Resize(size_ + Int64Len - (size_ % Int64Len));
    auto reverse_int64 = [](int64_t x) -> int64_t {
        x = ((x & 0x5555555555555555LL) << 1) | ((x >> 1) & 0x5555555555555555LL);    // NOLINT
        x = ((x & 0x3333333333333333LL) << 2) | ((x >> 2) & 0x3333333333333333LL);    // NOLINT
        x = ((x & 0x0F0F0F0F0F0F0F0FLL) << 4) | ((x >> 4) & 0x0F0F0F0F0F0F0F0FLL);    // NOLINT
        x = ((x & 0x00FF00FF00FF00FFLL) << 8) | ((x >> 8) & 0x00FF00FF00FF00FFLL);    // NOLINT
        x = ((x & 0x0000FFFF0000FFFFLL) << 16) | ((x >> 16) & 0x0000FFFF0000FFFFLL);  // NOLINT
        x = (x << 32) | (x >> 32);                                                    // NOLINT
        return x;
    };
    for (int64_t& val : result.bits_) {
        val = reverse_int64(val);
    }
    std::reverse(result.bits_.begin(), result.bits_.end());
    result = result >> (Int64Len - (size_ % Int64Len));
    result.Resize(size_);
    return result;
}

std::string Bitset::ToString() const {
    std::string result(size_, '0');
    for (size_t i = 0; i < size_; ++i) {
        if ((*this)[i]) {
            result[i] = '1';
        }
    }
    return result;
}