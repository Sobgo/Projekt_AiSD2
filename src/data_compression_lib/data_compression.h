#ifndef DATA_COMPRESSION_H
#define DATA_COMPRESSION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <string>

using namespace std;

// Node structure for the Huffman tree
struct Node {
	char ch;
	int freq;
	Node *left, *right;

	Node(char character, int frequency);
};

// Comparator for the priority queue
struct Compare {
	bool operator()(Node* left, Node* right);
};

// Function to generate Huffman codes
void generateCodes(Node* root, string str, unordered_map<char, string> &huffmanCode);

// Function to build the Huffman tree
Node* buildHuffmanTree(unordered_map<char, int> &freq);

// Function to save the encoded file
void saveEncodedFile(const string &filename, const string &encodedStr, unordered_map<char, string> &huffmanCode);

// Function to load the encoded file
void loadEncodedFile(const string &filename, string &encodedStr, unordered_map<char, string> &huffmanCode);

// Function to compress a text file
void compressFile(const string &inputFilename, const string &outputFilename);

// Function to decompress a file
void decompressFile(const string &inputFilename, const string &outputFilename);

#endif
