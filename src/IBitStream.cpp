//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include "IBitStream.h"

IBitStream::IBitStream(const std::string& file_path) {
    Open(file_path);
}

IBitStream::~IBitStream() {
    Close();
}

void IBitStream::Open(const std::string& file_path) {
    Close();
    if (!std::filesystem::exists(file_path)) {
        throw std::runtime_error("IBitStream::Open error: " + file_path + " doesn't exist");
    }
    file_.open(file_path, std::ifstream::binary);
}

void IBitStream::Close() {
    file_.close();
}

bool IBitStream::GetBit() {
    if (IsEof()) {
        throw std::runtime_error("IBitStream::GetBit error: can't read because of EOF");
    }
    if (buffer_pos_ == CharLen) {
        try {
            file_.get(buffer_);
        } catch (std::exception& e) {
            throw std::runtime_error("IBitStream::GetBit error: no opened file");
        }
        buffer_pos_ = 0;
    }
    return (buffer_ >> buffer_pos_++) & 1;
}

unsigned char IBitStream::GetByte() {
    Bitset chr(CharLen);
    try {
        Get(chr);
    } catch (std::exception& e) {
        throw std::runtime_error("IBitStream::GetByte error: can't read because of EOF");
    }
    return static_cast<char>(chr);
}

int16_t IBitStream::Get9Bit() {
    Bitset chr(ArchiveCharLen);
    try {
        Get(chr);
    } catch (std::exception& e) {
        throw std::runtime_error("IBitStream::Get9Bit error: can't read because of EOF");
    }
    return static_cast<int16_t>(chr);
}

void IBitStream::Get(Bitset& chr) {
    try {
        for (int32_t i = 0; i < chr.Size(); ++i) {
            chr.Set(i, GetBit());
        }
    } catch (std::exception& e) {
        throw std::runtime_error("IBitStream::Get error: can't read because of EOF");
    }
}

bool IBitStream::IsEof() {
    file_.peek();
    return file_.eof() && buffer_pos_ == CharLen;
}