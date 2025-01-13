//
// Created by Дмитрий Калугин on 08.10.2024.
//

#ifndef CPP_HSE_ARCHIVE_H
#define CPP_HSE_ARCHIVE_H

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include "IBitStream.h"
#include "OBitStream.h"
#include "Bitset.h"
#include "Trie.h"
#include "utils.h"

void Archive(const std::vector<std::string>& file_names, const std::string& archive_name);

#endif  // CPP_HSE_ARCHIVE_H
