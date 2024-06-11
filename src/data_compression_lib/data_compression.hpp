#ifndef DATA_COMPRESSION_HPP
#define DATA_COMPRESSION_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace data_compression {

using HuffmanCode = std::unordered_map<char, std::pair<uint32_t, int>>;

HuffmanCode generate_huffman_code(const std::string &text);

std::vector<uint8_t> compress(const std::string &text, const HuffmanCode &huffmanCode);

std::string decompress(const std::vector<uint8_t> &encodedText, const HuffmanCode &huffmanCode);

}

#endif
