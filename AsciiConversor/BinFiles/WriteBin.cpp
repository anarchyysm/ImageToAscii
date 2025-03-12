#include "WriteBin.hpp"
#include <fstream>

WriteBin::WriteBin(const std::string& filename) 
  : filename(filename){}

void WriteBin::saveASCIIFrames(const std::vector<std::string>& asciiFrames){
  std::ofstream outFile(filename, std::ios::binary);
  if(!outFile.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }

  size_t numFrames = asciiFrames.size();
  outFile.write(reinterpret_cast<const char*>(&numFrames), sizeof(numFrames));

  for (const std::string& frame : asciiFrames) {
    size_t frameSize = frame.size();
    outFile.write(reinterpret_cast<const char*>(&frameSize), sizeof(frameSize));

    outFile.write(frame.data(), frameSize);
  }

  outFile.close();
}
