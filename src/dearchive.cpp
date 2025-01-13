//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include "dearchive.h"

void Dearchive(const std::string& archive_name) {
    IBitStream is(archive_name);
    bool is_archive_end = false;
    do {
        // Количество символов
        int16_t char_cnt = is.Get9Bit();
        // Символы в порядке канонических кодов
        std::vector<std::pair<int16_t, int16_t>> codes_len_sorted;
        for (int16_t i = 0; i < char_cnt; ++i) {
            codes_len_sorted.emplace_back(is.Get9Bit(), 0);
        }
        // Количество символов с длиной кода
        int16_t char_filled = 0;
        int16_t cur_len = 1;
        while (char_filled != char_cnt) {
            int16_t len_cnt = is.Get9Bit();
            for (int16_t i = 0; i < len_cnt; ++i) {
                codes_len_sorted[char_filled++].second = cur_len;
            }
            ++cur_len;
        }
        // Восстановление бора
        Trie trie;
        try {
            std::unordered_map<int16_t, int16_t> codes_len(codes_len_sorted.begin(), codes_len_sorted.end());
            std::unordered_map<int16_t, Bitset> char_codes = CodesLenToCanonicalCodes(codes_len);
            for (const auto& [chr, code] : char_codes) {
                trie.AddNode(code, chr);
            }
        } catch (std::exception& e) {
            throw std::runtime_error("Dearchive error: archive incorrect");
        }
        // Чтение имени
        std::string file_path = std::filesystem::current_path().string() + "/";
        for (int16_t chr = ReadCode(is, trie); chr != FilenameEnd; chr = ReadCode(is, trie)) {
            if (Int16Len - __builtin_clzs(chr) > CharLen) {
                throw std::runtime_error("Dearchive error: ReadCode returned non-char value while name reading");
            }
            file_path += static_cast<char>(chr);
        }
        // Запись файла
        int16_t chr = ReadCode(is, trie);
        OBitStream os(file_path);
        while (chr != ArchiveEnd && chr != OneMoreFile && chr != FilenameEnd) {
            os.WriteByte(static_cast<char>(chr));
            chr = ReadCode(is, trie);
        }
        if (chr == FilenameEnd) {
            throw std::runtime_error("Dearchive error: ReadCode returned FILENAME_END char while decompressing");
        }
        if (chr == ArchiveEnd) {
            is_archive_end = true;
        }
    } while (!is_archive_end);
}