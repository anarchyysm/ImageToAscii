#ifndef WRITEBIN_HPP
#define WRITEBIN_HPP

#include "Dependencies.hpp"
#include <cstring>

class WriteBin {
  public:
    WriteBin(const std::string& filename);
    void saveASCIIFrames(const std::vector<std::string>& asciiFrames);

  private:
    std::string filename;
};

#endif // !WRITEBIN_HPP
