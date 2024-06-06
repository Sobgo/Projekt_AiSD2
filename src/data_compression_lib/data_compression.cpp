#include "data_compression.h"

#include <queue>
#include <bitset>
#include <functional>

struct Node {
	char ch;
	int freq;
	Node *left, *right;

	Node(char ch, int freq, Node* left = nullptr, Node* right = nullptr)
	    : ch(ch), freq(freq), left(left), right(right) {}

	~Node() {
		delete left;
		delete right;
	}
};

struct Compare {
	bool operator()(Node* l, Node* r) {
		return l->freq > r->freq;
	}
};

void buildHuffmanTree(const std::string& text, std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode) {
	std::unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
	for (auto& pair : freq) {
		pq.push(new Node(pair.first, pair.second));
	}

	while (pq.size() != 1) {
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top(); pq.pop();

		int sum = left->freq + right->freq;
		Node *newNode = new Node('\0', sum);
		pq.push(newNode);
	}

	Node* root = pq.top();
	std::unordered_map<char, std::string> tempCode;
	std::string str;
	std::function<void(Node*, std::string)> encode = [&](Node* node, std::string str) {
		if (!node) return;
		if (!node->left && !node->right) {
			tempCode[node->ch] = str;
		}
		encode(node->left, str + "0");
		encode(node->right, str + "1");
	};
	encode(root, str);

	std::vector<std::pair<char, std::string>> sortedCodes(tempCode.begin(), tempCode.end());
	std::sort(sortedCodes.begin(), sortedCodes.end(), [](auto &left, auto &right) {
		return left.second.length() < right.second.length() ||
		       (left.second.length() == right.second.length() && left.first < right.first);
	});

	huffmanCode.clear();
	uint32_t code = 0;
	int prevLen = 0;
	for (auto& pair : sortedCodes) {
		int len = pair.second.length();
		code <<= (len - prevLen);
		huffmanCode[pair.first] = {code, len};
		code++;
		prevLen = len;
	}

	delete root;
}

std::vector<uint8_t> compress(const std::string& text, std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode) {
	buildHuffmanTree(text, huffmanCode);
	std::vector<uint8_t> compressedText;
	uint32_t buffer = 0;
	int bitCount = 0;

	for (char ch : text) {
		auto [code, len] = huffmanCode[ch];
		buffer <<= len;
		buffer |= code;
		bitCount += len;

		while (bitCount >= 8) {
			bitCount -= 8;
			compressedText.push_back(buffer >> bitCount);
			buffer &= (1 << bitCount) - 1;
		}
	}

	if (bitCount > 0) {
		buffer <<= (8 - bitCount);
		compressedText.push_back(buffer);
	}

	return compressedText;
}

std::string decompress(const std::vector<uint8_t>& encodedText, const std::unordered_map<char, std::pair<uint32_t, int>>& huffmanCode) {
	std::unordered_map<uint32_t, std::pair<char, int>> reverseHuffmanCode;
	for (auto &pair : huffmanCode) {
		reverseHuffmanCode[pair.second.first] = {pair.first, pair.second.second};
	}

	std::string decodedText;
	uint32_t buffer = 0;
	int bitCount = 0;

	for (uint8_t byte : encodedText) {
		buffer = (buffer << 8) | byte;
		bitCount += 8;

		while (bitCount >= 8) {
			bool found = false;
			for (int len = 1; len <= 32 && len <= bitCount; len++) {
				uint32_t code = buffer >> (bitCount - len);
				auto it = reverseHuffmanCode.find(code);
				if (it != reverseHuffmanCode.end() && it->second.second == len) {
					decodedText += it->second.first;
					bitCount -= len;
					buffer &= (1 << bitCount) - 1;
					found = true;
					break;
				}
			}
			if (!found) break;
		}
	}

	return decodedText;
}
