//
// Created by Дмитрий Калугин on 11.10.2024.
//

#include <fstream>
#include <cstdint>
#include <string>
#include <stdexcept>
#include "consts.h"
#include "Bitset.h"

#ifndef CPP_HSE_OBITSTREAM_H
#define CPP_HSE_OBITSTREAM_H

class OBitStream {
private:
    char buffer_ = 0;
    char buffer_len_ = 0;
    std::ofstream file_;

public:
    explicit OBitStream(const std::string& file_path);

    ~OBitStream();

    void Open(const std::string& file_path);

    void Close();

    void Flush();

    void WriteBit(bool b);

    void WriteByte(unsigned char c);

    void Write9Bit(int16_t c);

    void Write(const Bitset& word);
};

#endif  // CPP_HSE_OBITSTREAM_H
