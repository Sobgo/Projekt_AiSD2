#include "data_compression.h"

Node::Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

bool Compare::operator()(Node* left, Node* right) {
	return left->freq > right->freq;
}

void generateCodes(Node* root, string str, unordered_map<char, string> &huffmanCode) {
	if (!root) return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	generateCodes(root->left, str + "0", huffmanCode);
	generateCodes(root->right, str + "1", huffmanCode);
}

Node* buildHuffmanTree(unordered_map<char, int> &freq) {
	priority_queue<Node*, vector<Node*>, Compare> pq;

	for (auto pair : freq) {
		pq.push(new Node(pair.first, pair.second));
	}

	while (pq.size() != 1) {
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top(); pq.pop();

		int sum = left->freq + right->freq;
		Node *newNode = new Node('\0', sum);
		newNode->left = left;
		newNode->right = right;
		pq.push(newNode);
	}

	return pq.top();
}

void saveEncodedFile(const string &filename, const string &encodedStr, unordered_map<char, string> &huffmanCode) {
	ofstream outFile(filename, ios::binary);

	// Write the Huffman codes map size
	size_t mapSize = huffmanCode.size();
	outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

	// Write the Huffman codes
	for (auto pair : huffmanCode) {
		outFile.write(&pair.first, sizeof(pair.first));
		size_t codeSize = pair.second.size();
		outFile.write(reinterpret_cast<char*>(&codeSize), sizeof(codeSize));
		outFile.write(pair.second.c_str(), codeSize);
	}

	// Write the encoded string size
	size_t encodedSize = encodedStr.size();
	outFile.write(reinterpret_cast<char*>(&encodedSize), sizeof(encodedSize));

	// Write the encoded string
	outFile.write(encodedStr.c_str(), encodedSize);

	outFile.close();
}

void loadEncodedFile(const string &filename, string &encodedStr, unordered_map<char, string> &huffmanCode) {
	ifstream inFile(filename, ios::binary);

	// Read the Huffman codes map size
	size_t mapSize;
	inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

	// Read the Huffman codes
	for (size_t i = 0; i < mapSize; ++i) {
		char ch;
		inFile.read(&ch, sizeof(ch));
		size_t codeSize;
		inFile.read(reinterpret_cast<char*>(&codeSize), sizeof(codeSize));
		string code(codeSize, ' ');
		inFile.read(&code[0], codeSize);
		huffmanCode[ch] = code;
	}

	// Read the encoded string size
	size_t encodedSize;
	inFile.read(reinterpret_cast<char*>(&encodedSize), sizeof(encodedSize));

	// Read the encoded string
	encodedStr.resize(encodedSize);
	inFile.read(&encodedStr[0], encodedSize);

	inFile.close();
}

void compressFile(const string &inputFilename, const string &outputFilename) {
	ifstream inFile(inputFilename);
	if (!inFile.is_open()) {
		cerr << "Error opening input file" << endl;
		return;
	}

	unordered_map<char, int> freq;
	string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
	inFile.close();

	for (char ch : text) {
		freq[ch]++;
	}

	Node* root = buildHuffmanTree(freq);
	unordered_map<char, string> huffmanCode;
	generateCodes(root, "", huffmanCode);

	string encodedStr;
	for (char ch : text) {
		encodedStr += huffmanCode[ch];
	}

	saveEncodedFile(outputFilename, encodedStr, huffmanCode);
}

void decompressFile(const string &inputFilename, const string &outputFilename) {
	unordered_map<char, string> huffmanCode;
	string encodedStr;
	loadEncodedFile(inputFilename, encodedStr, huffmanCode);

	unordered_map<string, char> reversedHuffmanCode;
	for (auto pair : huffmanCode) {
		reversedHuffmanCode[pair.second] = pair.first;
	}

	string currentCode;
	string decompressedText;
	for (char bit : encodedStr) {
		currentCode += bit;
		if (reversedHuffmanCode.find(currentCode) != reversedHuffmanCode.end()) {
			decompressedText += reversedHuffmanCode[currentCode];
			currentCode.clear();
		}
	}

	ofstream outFile(outputFilename);
	if (!outFile.is_open()) {
		cerr << "Error opening output file" << endl;
		return;
	}

	outFile << decompressedText;
	outFile.close();
}
