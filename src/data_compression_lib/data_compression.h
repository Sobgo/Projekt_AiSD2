#ifndef DATA_COMPRESSION_H
#define DATA_COMPRESSION_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

struct Node;

void buildHuffmanTree(const std::string& text, std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode);
std::vector<uint8_t> compress(const std::string& text, std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode);
std::string decompress(const std::vector<uint8_t>& encodedText, std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode);

#endif
