//
// Created by Дмитрий Калугин on 11.10.2024.
//

#include "OBitStream.h"

OBitStream::OBitStream(const std::string& file_path) {
    Open(file_path);
}

OBitStream::~OBitStream() {
    Flush();
    file_.close();
}

void OBitStream::Open(const std::string& file_path) {
    file_.close();
    file_.open(file_path);
}

void OBitStream::Close() {
    Flush();
    file_.close();
}

void OBitStream::Flush() {
    file_.put(buffer_);
    buffer_ = 0;
    buffer_len_ = 0;
}

void OBitStream::WriteBit(bool b) {
    if (buffer_len_ == CharLen) {
        file_.put(buffer_);
        buffer_ = 0;
        buffer_len_ = 0;
    }
    buffer_ = static_cast<char>(buffer_ + (b * (1 << buffer_len_++)));
}

void OBitStream::WriteByte(unsigned char c) {
    for (char i = 0; i < CharLen; ++i) {
        WriteBit((c >> i) & 1);
    }
}

void OBitStream::Write9Bit(int16_t c) {
    for (char i = 0; i < ArchiveCharLen; ++i) {
        WriteBit((c >> i) & 1);
    }
}

void OBitStream::Write(const Bitset& word) {
    for (int32_t i = 0; i < word.Size(); ++i) {
        WriteBit(word[i]);
    }
}