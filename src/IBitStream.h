//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include <fstream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <filesystem>
#include "consts.h"
#include "Bitset.h"

#ifndef CPP_HSE_IBITSTREAM_H
#define CPP_HSE_IBITSTREAM_H

class IBitStream {
private:
    char buffer_ = 0;
    char buffer_pos_ = CharLen;
    std::ifstream file_;

public:
    explicit IBitStream(const std::string& file_path);

    ~IBitStream();

    void Open(const std::string& file_path);

    void Close();

    bool GetBit();

    unsigned char GetByte();

    int16_t Get9Bit();

    void Get(Bitset& chr);

    bool IsEof();
};

#endif  // CPP_HSE_IBITSTREAM_H
