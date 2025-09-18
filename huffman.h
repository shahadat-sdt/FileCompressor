#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

namespace Huffman {
    bool compressFile(const std::string &inputPath, const std::string &outputPath, std::string &outErr);
    bool decompressFile(const std::string &inputPath, const std::string &outputPath, std::string &outErr);
}

#endif // HUFFMAN_H
