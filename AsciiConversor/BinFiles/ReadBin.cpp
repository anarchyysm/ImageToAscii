#include "ReadBin.hpp"
#include <fstream>
#include <stdexcept>

ReadBin::ReadBin(const std::string& filename) : filename(filename) {}

std::vector<std::string> ReadBin::loadASCIIFrames() {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    // Ler o número de frames
    size_t numFrames;
    inFile.read(reinterpret_cast<char*>(&numFrames), sizeof(numFrames));

    std::vector<std::string> asciiFrames;
    asciiFrames.reserve(numFrames);

    // Para cada frame
    for (size_t i = 0; i < numFrames; ++i) {
        // Ler o tamanho da string
        size_t frameSize;
        inFile.read(reinterpret_cast<char*>(&frameSize), sizeof(frameSize));

        // Ler os dados da string
        std::vector<char> buffer(frameSize);
        inFile.read(buffer.data(), frameSize);

        std::string frame(buffer.begin(), buffer.end());
        asciiFrames.push_back(frame);
    }

    inFile.close();
    return asciiFrames;
}
