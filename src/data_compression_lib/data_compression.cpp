#include "data_compression.hpp"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// TODO: Improve memory management
// NOLINTBEGIN(cppcoreguidelines-special-member-functions)
// NOLINTBEGIN(cppcoreguidelines-owning-memory)

namespace data_compression {

struct Node {
	char ch;
	int freq;
	Node *left, *right;

	Node(char ch = '\0', int freq = 0, Node *left = nullptr, Node *right = nullptr)
	    : ch(ch), freq(freq), left(left), right(right) {}

	~Node() {
		if (left != nullptr) delete left;
		if (right != nullptr) delete right;
	}
};

struct Compare {
	bool operator()(Node *l, Node *r) { return l->freq > r->freq; }
};

HuffmanCode generate_huffman_code(const std::string &text) {
	if (text.empty()) return {};
	std::unordered_map<char, int> freq;
	for (const char ch : text) {
		freq[ch]++;
	}

	std::priority_queue<Node *, std::vector<Node *>, Compare> pq;
	for (auto &pair : freq) {
		pq.push(new Node(pair.first, pair.second));
	}

	while (pq.size() != 1) {
		Node *left = pq.top();
		pq.pop();
		Node *right = pq.top();
		pq.pop();

		const int sum = left->freq + right->freq;
		Node *newNode = new Node('\0', sum);

		newNode->left = left;
		newNode->right = right;
		pq.push(newNode);
	}

	Node *root = pq.top();
	if (root->ch != '\0') {
		Node *newRoot = new Node('\0', root->freq);
		newRoot->left = root;
		root = newRoot;
	}

	std::unordered_map<char, std::string> tempCode;
	std::function<void(Node *, std::string)> encode = [&](Node *node, const std::string &str) {
		if (node == nullptr) return;
		if (node->left == nullptr && node->right == nullptr) {
			tempCode[node->ch] = str;
		}
		encode(node->left, str + "0");
		encode(node->right, str + "1");
	};
	encode(root, "");

	std::vector<std::pair<char, std::string>> sortedCodes(tempCode.begin(), tempCode.end());
	std::sort(sortedCodes.begin(), sortedCodes.end(), [](auto &left, auto &right) {
		return left.second.length() < right.second.length() ||
		       (left.second.length() == right.second.length() && left.first < right.first);
	});

	HuffmanCode huffmanCode;
	uint32_t code = 0;
	int prevLen = 0;
	for (auto &pair : sortedCodes) {
		const int len = pair.second.length();
		code <<= (len - prevLen);
		huffmanCode[pair.first] = {code, len};
		code++;
		prevLen = len;
	}

	delete root;

	return huffmanCode;
}

std::vector<bool> compress(const std::string &text, const HuffmanCode &huffmanCode) {
	std::vector<bool> compressedText;

	for (const char ch : text) {
		auto [code, len] = huffmanCode.at(ch);
		for (int i = len - 1; i >= 0; i--) {
			compressedText.push_back((code >> i) & 1);
		}
	}

	return compressedText;
}

std::string decompress(const std::vector<bool> &encodedText, const HuffmanCode &huffmanCode) {
	std::string decodedText;

	Node root;
	for (const auto [ch, code_len] : huffmanCode) {
		const auto [code, len] = code_len;
		Node *current = &root;
		for (int i = len - 1; i >= 0; i--) {
			if ((code >> i) & 1) {
				if (current->right == nullptr) {
					current->right = new Node();
				}
				current = current->right;
			} else {
				if (current->left == nullptr) {
					current->left = new Node();
				}
				current = current->left;
			}
		}
		current->ch = ch;
	}

	Node *current = &root;
	for (const bool bit : encodedText) {
		if (bit) {
			current = current->right;
		} else {
			current = current->left;
		}

		if (current == nullptr) throw std::runtime_error("Invalid encoded text");

		if (current->ch != '\0') {
			decodedText += current->ch;
			current = &root;
		}
	}

	return decodedText;
}

// NOLINTEND(cppcoreguidelines-owning-memory)
// NOLINTEND(cppcoreguidelines-special-member-functions)

}
