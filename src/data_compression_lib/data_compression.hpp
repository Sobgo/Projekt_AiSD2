#ifndef DATA_COMPRESSION_HPP
#define DATA_COMPRESSION_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace data_compression {

struct Node;

void buildHuffmanTree(const std::string &text,
                      std::unordered_map<char, std::pair<uint32_t, int>> huffmanCode);
std::vector<uint8_t> compress(const std::string &text,
                              std::unordered_map<char, std::pair<uint32_t, int>> huffmanCode);
std::string decompress(const std::vector<uint8_t> &encodedText,
                       const std::unordered_map<char, std::pair<uint32_t, int>> &huffmanCode);

}

#endif
