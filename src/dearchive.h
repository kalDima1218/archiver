//
// Created by Дмитрий Калугин on 08.10.2024.
//

#ifndef CPP_HSE_DEARCHIVE_H
#define CPP_HSE_DEARCHIVE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <filesystem>
#include "IBitStream.h"
#include "OBitStream.h"
#include "utils.h"
#include "Trie.h"

void Dearchive(const std::string& file_name);

#endif  // CPP_HSE_DEARCHIVE_H
