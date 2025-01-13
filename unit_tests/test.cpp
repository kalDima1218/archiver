#include "catch.hpp"

#include "../src/Bitset.h"
#include "../src/Heap.h"
#include "../src/IBitStream.h"
#include "../src/OBitStream.h"
#include "../src/Trie.h"
#include "../src/utils.h"

TEST_CASE("Heap_min") {
    Heap<int, std::less<int>> heap;
    heap = {2, 4, 2, 5, 3, 6, 2, 6, 8, 9, 5, 3, 1};
    std::vector<int> heap_must_be = {2, 4, 2, 5, 3, 6, 2, 6, 8, 9, 5, 3, 1};
    std::sort(heap_must_be.begin(), heap_must_be.end());
    std::vector<int> heap_is;
    while (!heap.IsEmpty()) {
        heap_is.push_back(heap.Top());
        heap.Pop();
    }
    REQUIRE(heap_is == heap_must_be);
}

TEST_CASE("Heap_max") {
    Heap<int, std::greater<int>> heap;
    heap = {2, 4, 2, 5, 3, 6, 2, 6, 8, 9, 5, 3, 1};
    std::vector<int> heap_must_be = {2, 4, 2, 5, 3, 6, 2, 6, 8, 9, 5, 3, 1};
    std::sort(heap_must_be.begin(), heap_must_be.end(), std::greater<int>());
    std::vector<int> heap_is;
    while (!heap.IsEmpty()) {
        heap_is.push_back(heap.Top());
        heap.Pop();
    }
    REQUIRE(heap_is == heap_must_be);
}

TEST_CASE("Bitset::operator char()") {
    Bitset bitset(8);
    bitset.Set(1, 1);
    bitset.Set(7, 1);
    REQUIRE(static_cast<char>(bitset) == 2-128);
}

TEST_CASE("Bitset::operator int16_t()") {
    Bitset bitset(16);
    bitset.Set(1, 1);
    bitset.Set(7, 1);
    bitset.Set(8, 1);
    REQUIRE(static_cast<int16_t>(bitset) == 2+128+256);
}

TEST_CASE("Bitset::operator int32_t()") {
    Bitset bitset(32);
    bitset.Set(1, 1);
    bitset.Set(7, 1);
    bitset.Set(8, 1);
    bitset.Set(20, 1);
    REQUIRE(static_cast<int32_t>(bitset) == 2+128+256+(1<<20));
}

TEST_CASE("Bitset::operator int64_t()") {
    Bitset bitset(64);
    bitset.Set(1, 1);
    bitset.Set(7, 1);
    bitset.Set(8, 1);
    bitset.Set(20, 1);
    bitset.Set(60, 1);
    REQUIRE(static_cast<int64_t>(bitset) == 2+128+256+(1<<20)+(1LL<<60LL));
}

TEST_CASE("Bitset::Set") {
    Bitset bitset(246);
    std::string bitset_must_be;
    for (size_t i = 0; i < 123; ++i) {
        bitset.Set(2 * i, 0);
        bitset.Set(2 * i + 1, 1);
        bitset_must_be += "01";
    }
    REQUIRE(bitset.ToString() == bitset_must_be);
}

TEST_CASE("Bitset::Resize_bigger") {
    Bitset bitset(123);
    std::string bitset_must_be;
    for (size_t i = 0; i < 100; ++i) {
        bitset.Set(i, 1);
        bitset_must_be += "1";
    }
    for (size_t i = 100; i < 200; ++i) {
        bitset_must_be += "0";
    }
    bitset.Resize(246);
    for (size_t i = 200; i < 246; ++i) {
        bitset.Set(i, 1);
        bitset_must_be += "1";
    }
    REQUIRE(bitset.ToString() == bitset_must_be);
}

TEST_CASE("Bitset::Resize_smaller") {
    Bitset bitset(123);
    std::string bitset_must_be;
    for (size_t i = 0; i < 100; ++i) {
        bitset_must_be += "0";
    }
    for (size_t i = 100; i < 110; ++i) {
        bitset_must_be += "1";
    }
    for (size_t i = 100; i < 123; ++i) {
        bitset.Set(i, 1);
    }
    bitset.Resize(110);
    REQUIRE(bitset.ToString() == bitset_must_be);
}

TEST_CASE("Bitset::operator[]") {
    Bitset bitset(246);
    std::string bitset_must_be;
    for (size_t i = 0; i < 123; ++i) {
        bitset.Set(2 * i, 0);
        bitset.Set(2 * i + 1, 1);
        bitset_must_be += "01";
    }
    std::string bitset_is;
    for (size_t i = 0; i < 246; ++i) {
        bitset_is += std::to_string(bitset[i]);
    }
    REQUIRE(bitset_is == bitset_must_be);
}

TEST_CASE("Bitset::operator++") {
    Bitset bitset(8);
    for (int i = 0; i < 125; ++i) {
        ++bitset;
    }
    REQUIRE(static_cast<char>(bitset) == 125);
}

TEST_CASE("Bitset::operator>>") {
    Bitset bitset(246);
    bitset.Set(60, 1);
    bitset.Set(120, 1);
    bitset.Set(180, 1);
    bitset.Set(240, 1);
    bitset = bitset >> 60;
    std::string bitset_must_be;
    for (size_t i = 0; i < 246; ++i) {
        bitset_must_be += "0";
    }
    bitset_must_be[0] = '1';
    bitset_must_be[60] = '1';
    bitset_must_be[120] = '1';
    bitset_must_be[180] = '1';
    REQUIRE(bitset.ToString() == bitset_must_be);
}

TEST_CASE("Bitset::operator<<") {
    Bitset bitset(246);
    bitset.Set(0, 1);
    bitset.Set(60, 1);
    bitset.Set(120, 1);
    bitset.Set(180, 1);
    bitset = bitset << 60;
    std::string bitset_must_be;
    for (size_t i = 0; i < 246; ++i) {
        bitset_must_be += "0";
    }
    bitset_must_be[60] = '1';
    bitset_must_be[120] = '1';
    bitset_must_be[180] = '1';
    bitset_must_be[240] = '1';
    REQUIRE(bitset.ToString() == bitset_must_be);
}

TEST_CASE("Bitset::operator<>") {
    Bitset bitset(8);
    for (int i = 0; i < 125; ++i) {
        ++bitset;
    }
    REQUIRE(static_cast<char>(bitset) > 120);
    REQUIRE(static_cast<char>(bitset) < 127);
}

TEST_CASE("Bitset::Reversed") {
    Bitset bitset(246);
    std::string bitset_must_be;
    for (size_t i = 0; i < 123; ++i) {
        bitset.Set(2 * i, 0);
        bitset.Set(2 * i + 1, 1);
        bitset_must_be += "10";
    }
    REQUIRE(bitset.Reversed().ToString() == bitset_must_be);
}

TEST_CASE("IBitStream") {
    std::string s = "Hello, World!\n";
    std::ofstream os("input.txt");
    for (char c : s) {
        os.put(c);
    }
    os.close();
    IBitStream is("input.txt");
    std::string file;
    while (!is.IsEof()) {
        file += is.GetByte();
    }
    REQUIRE(file == s);
}

TEST_CASE("OBitStream") {
    OBitStream os("output.txt");
    std::string s = "Hello, World!\n";
    for (char chr : s) {
        os.WriteByte(chr);
    }
    os.Close();
    std::ifstream is("output.txt");
    std::string s_read;
    char chr;
    is.peek();
    while (!is.eof()) {
        is.get(chr);
        s_read += chr;
        is.peek();
    }
    REQUIRE(s_read == s);
}

TEST_CASE("Trie") {
    std::unordered_map<int16_t, int64_t> char_freq;
    for (int16_t i = 0; i < 16000; ++i) {
        char_freq[i] = __builtin_popcount(i);
    }
    Trie trie;
    std::unordered_map<int16_t, Bitset> char_codes = CodesLenToCanonicalCodes(GetCodesLen(char_freq));
    for (const auto& [chr, code] : char_codes) {
        trie.AddNode(code, chr);
    }
    for (const auto& [chr, code] : char_codes) {
        std::shared_ptr<Node> v = trie.GetRoot();
        for(size_t i = 0; i < code.Size(); ++i){
            REQUIRE(v->is_terminate == false);
            if(code[i]){
                REQUIRE(v->right != nullptr);
                v = v->right;
            } else {
                REQUIRE(v->left != nullptr);
                v = v->left;
            }
        }
        REQUIRE(v->is_terminate == true);
    }
}