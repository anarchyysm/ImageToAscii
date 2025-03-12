#ifndef READBIN_HPP
#define READBIN_HPP

#include "Dependencies.hpp"

class ReadBin {
public:
    ReadBin(const std::string& filename);
    std::vector<std::string> loadASCIIFrames();

private:
    std::string filename;
};

#endif // READBIN_HPP
