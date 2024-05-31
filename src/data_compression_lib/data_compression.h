#ifndef DATA_COMPRESSION_HPP
#define DATA_COMPRESSION_HPP

#include <unordered_map>
#include <string>

class HuffmanNode {
  public:
	char char_;
	int freq;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(int freq, char char_ = '\0', HuffmanNode* left = nullptr, HuffmanNode* right = nullptr);
	bool isLeaf() const;

	bool operator<(const HuffmanNode& other) const;
};

HuffmanNode* buildHTree(const std::unordered_map<char, int>& freqData);
std::unordered_map<char, std::string> hTreeToHCode(HuffmanNode* hTree);
std::unordered_map<char, int> countFrequency(const std::string& s);
std::string encode(const std::string& s, const std::unordered_map<char, int>& freqData);
std::string decode(const std::string& s, const std::unordered_map<char, int>& freqData);
void saveToFile(const std::string& fileName, const std::string& data);

#endif
