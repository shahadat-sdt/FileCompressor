#include "huffman.h"
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <bitset>
#include <sstream>

struct Node {
    unsigned char ch;
    uint64_t freq;
    Node* left;
    Node* right;

    Node(unsigned char c, uint64_t f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) const { return a->freq > b->freq; }
};

static void buildCodes(Node* node, const std::string &prefix, std::unordered_map<unsigned char, std::string> &codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = prefix.size() ? prefix : "0";
    }
    buildCodes(node->left, prefix + "0", codes);
    buildCodes(node->right, prefix + "1", codes);
}

static void freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

static void writeUint64(std::ofstream &ofs, uint64_t v) {
    for (int i=0; i<8; i++) {
        ofs.put((v >> (i*8)) & 0xFF);
    }
}

static bool readUint64(std::ifstream &ifs, uint64_t &v) {
    v = 0;
    for (int i=0; i<8; i++) {
        char c;
        if (!ifs.get(c)) return false;
        v |= (uint64_t(static_cast<unsigned char>(c)) << (i*8));
    }
    return true;
}

bool Huffman::compressFile(const std::string &inputPath, const std::string &outputPath, std::string &outErr) {
    std::ifstream ifs(inputPath, std::ios::binary);
    if (!ifs.is_open()) { outErr = "Cannot open input file"; return false; }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    uint64_t freq[256] = {0};
    for (unsigned char uc : content) freq[uc]++;

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (int i=0; i<256; i++) if (freq[i]) pq.push(new Node((unsigned char)i, freq[i]));

    if (pq.empty()) { outErr = "Empty file"; return false; }

    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(l, r));
    }
    Node* root = pq.top();

    std::unordered_map<unsigned char,std::string> codes;
    buildCodes(root, "", codes);

    std::ofstream ofs(outputPath, std::ios::binary);
    if (!ofs.is_open()) { freeTree(root); outErr = "Cannot open output file"; return false; }

    writeUint64(ofs, content.size());
    uint64_t distinct = 0;
    for (int i=0;i<256;i++) if(freq[i]) ++distinct;
    writeUint64(ofs, distinct);

    for (int i=0;i<256;i++) {
        if(freq[i]) {
            ofs.put(i);
            writeUint64(ofs, freq[i]);
        }
    }

    std::string bitBuffer;
    for (unsigned char uc : content) bitBuffer += codes[uc];

    writeUint64(ofs, bitBuffer.size());

    uint8_t byteVal = 0;
    int bitPos = 0;
    for (char b : bitBuffer) {
        byteVal = (byteVal << 1) | (b-'0');
        bitPos++;
        if (bitPos == 8) { ofs.put(byteVal); bitPos = 0; byteVal = 0; }
    }
    if (bitPos > 0) { byteVal <<= (8 - bitPos); ofs.put(byteVal); }

    ofs.close();
    freeTree(root);
    return true;
}

bool Huffman::decompressFile(const std::string &inputPath, const std::string &outputPath, std::string &outErr) {
    std::ifstream ifs(inputPath, std::ios::binary);
    if (!ifs.is_open()) { outErr = "Cannot open input file"; return false; }

    uint64_t originalSize;
    if (!readUint64(ifs, originalSize)) { outErr="Corrupt file"; return false; }

    uint64_t distinct;
    if(!readUint64(ifs, distinct)) { outErr="Corrupt file"; return false; }

    uint64_t freq[256] = {0};
    for(uint64_t i=0;i<distinct;i++) {
        char sym;
        if (!ifs.get(sym)) { outErr="Corrupt file"; return false; }
        uint64_t f;
        if(!readUint64(ifs,f)) { outErr="Corrupt file"; return false; }
        freq[static_cast<unsigned char>(sym)] = f;
    }

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (int i=0;i<256;i++) if(freq[i]) pq.push(new Node((unsigned char)i,freq[i]));

    while(pq.size()>1){
        Node* l=pq.top(); pq.pop();
        Node* r=pq.top(); pq.pop();
        pq.push(new Node(l,r));
    }
    Node* root = pq.top();

    uint64_t bitCount;
    if(!readUint64(ifs,bitCount)) { freeTree(root); outErr="Corrupt file"; return false; }

    std::string bits;
    bits.reserve(bitCount);
    while(bits.size()<bitCount){
        char byteChar;
        if(!ifs.get(byteChar)) break;
        uint8_t val = static_cast<uint8_t>(byteChar);
        for(int i=7;i>=0 && bits.size()<bitCount;i--) bits.push_back(((val>>i)&1)?'1':'0');
    }
    ifs.close();

    std::ofstream ofs(outputPath, std::ios::binary);
    if(!ofs.is_open()) { freeTree(root); outErr="Cannot write output"; return false; }

    Node* node = root;
    uint64_t written=0;
    for(char b : bits){
        node = (b=='0')?node->left:node->right;
        if(!node->left && !node->right){
            ofs.put(node->ch);
            node=root;
            if(++written>=originalSize) break;
        }
    }
    ofs.close();
    freeTree(root);
    return true;
}
