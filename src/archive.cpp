//
// Created by Дмитрий Калугин on 08.10.2024.
//

#include "archive.h"

void Archive(const std::vector<std::string>& file_paths, const std::string& archive_name) {
    OBitStream os(archive_name);
    for (int32_t file_index = 0; file_index < file_paths.size(); ++file_index) {
        std::filesystem::path file_path = file_paths[file_index];
        std::string file_name = file_path.filename().string();
        std::unordered_map<int16_t, int16_t> codes_len = GetCodesLen(CalcFreq(file_path));
        std::unordered_map<int16_t, Bitset> char_codes = CodesLenToCanonicalCodes(codes_len);
        // Количество символов
        os.Write9Bit(static_cast<int16_t>(char_codes.size()));
        // Символы в порядке канонических кодов
        std::vector<std::pair<int16_t, int16_t>> codes_len_sorted;
        for (auto [chr, len] : codes_len) {
            codes_len_sorted.emplace_back(len, chr);
        }
        std::sort(codes_len_sorted.begin(), codes_len_sorted.end());
        for (auto [len, chr] : codes_len_sorted) {
            os.Write9Bit(chr);
        }
        // Количество символов с длиной кода
        std::vector<int16_t> len_cnt(codes_len_sorted.back().first);
        for (auto [len, _] : codes_len_sorted) {
            ++len_cnt[len - 1];
        }
        for (int16_t cnt : len_cnt) {
            os.Write9Bit(cnt);
        }
        // Кодирование имени файла
        for (char c : file_name) {
            os.Write(char_codes[c]);
        }
        os.Write(char_codes[FilenameEnd]);
        // Кодирование содержимого
        IBitStream is(file_path);
        while (!is.IsEof()) {
            os.Write(char_codes[is.GetByte()]);
        }
        // Кодирование конца раздела
        if (file_index == file_paths.size() - 1) {
            os.Write(char_codes[ArchiveEnd]);
        } else {
            os.Write(char_codes[OneMoreFile]);
        }
    }
}