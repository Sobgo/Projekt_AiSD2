#include "data_compression.h"
#include <queue>
#include <fstream>
#include <iostream>

HuffmanNode::HuffmanNode(int freq, char char_, HuffmanNode* left, HuffmanNode* right)
    : char_(char_), freq(freq), left(left), right(right) {}

bool HuffmanNode::isLeaf() const {
	return (left == nullptr && right == nullptr);
}

bool HuffmanNode::operator<(const HuffmanNode& other) const {
	return freq > other.freq; // To make the priority queue a min-heap
}

HuffmanNode* buildHTree(const std::unordered_map<char, int>& freqData) {
	std::priority_queue<HuffmanNode> huffmanNodes;
	for (const auto& pair : freqData) {
		huffmanNodes.push(HuffmanNode(pair.second, pair.first));
	}

	while (huffmanNodes.size() > 1) {
		HuffmanNode* child1 = new HuffmanNode(huffmanNodes.top());
		huffmanNodes.pop();
		HuffmanNode* child2 = new HuffmanNode(huffmanNodes.top());
		huffmanNodes.pop();
		HuffmanNode parent(child1->freq + child2->freq, '\0', child1, child2);
		huffmanNodes.push(parent);
	}

	return new HuffmanNode(huffmanNodes.top());
}

void getCode(HuffmanNode* hNode, std::unordered_map<char, std::string>& code, const std::string& curCode = "") {
	if (hNode == nullptr) return;
	if (hNode->left == nullptr && hNode->right == nullptr) {
		code[hNode->char_] = curCode;
	}
	getCode(hNode->left, code, curCode + "0");
	getCode(hNode->right, code, curCode + "1");
}

std::unordered_map<char, std::string> hTreeToHCode(HuffmanNode* hTree) {
	std::unordered_map<char, std::string> code;
	getCode(hTree, code);
	return code;
}

std::unordered_map<char, int> countFrequency(const std::string& s) {
	std::unordered_map<char, int> freqData;
	for (char char_ : s) {
		freqData[char_]++;
	}
	return freqData;
}

std::string encode(const std::string& s, const std::unordered_map<char, int>& freqData) {
	HuffmanNode* hTree = buildHTree(freqData);
	std::unordered_map<char, std::string> hCode = hTreeToHCode(hTree);
	std::string hEncoded = "";
	for (char char_ : s) {
		hEncoded += hCode[char_];
	}
	return hEncoded;
}

std::string decode(const std::string& s, const std::unordered_map<char, int>& freqData) {
	HuffmanNode* hTree = buildHTree(freqData);
	std::string decodedStr = "";
	HuffmanNode* curTreeNode = hTree;
	for (char charCode : s) {
		if (charCode == '0') {
			curTreeNode = curTreeNode->left;
		} else {
			curTreeNode = curTreeNode->right;
		}
		if (curTreeNode->isLeaf()) {
			decodedStr += curTreeNode->char_;
			curTreeNode = hTree;
		}
	}
	return decodedStr;
}

void saveToFile(const std::string& fileName, const std::string& data) {
	std::ofstream outputFile(fileName);
	if (outputFile.is_open()) {
		outputFile << data;
		outputFile.close();
		std::cout << "Data saved to " << fileName << std::endl;
	} else {
		std::cerr << "Unable to open file " << fileName << std::endl;
	}
}

/*
 * EXAMPLE MAIN
 *
int main() {
std::ifstream inputFile("input.txt");
if (!inputFile.is_open()) {
	std::cerr << "Unable to open file input.txt";
	return 1;
}

std::string toEncode;
char ch;
while (inputFile.get(ch)) {
toEncode += ch;
}
inputFile.close();

std::unordered_map<char, int> freqData = countFrequency(toEncode);

std::string encodedStr = encode(toEncode, freqData);
saveToFile("encoded.txt", encodedStr);

std::string decodedStr = decode(encodedStr, freqData);
saveToFile("decoded.txt", decodedStr);

return 0;
}
 *
 */