#pragma once 

#include "type.hpp"
#include <vector>
#include <FileManager.hpp>
#include <sodium.h>

namespace fs = std::filesystem;

class cryptoManager
{
private:
    Config _conf;
    std::vector<fs::path> _files;

    unsigned char _symmetricKey[SYMMETRIC_KEY_SIZE];
    char _hex_symmetricKey[HEX_SYMMETRIC_KEY_SIZE];


public:
    cryptoManager();
    cryptoManager(Config conf, std::vector<fs::path> files);
    ~cryptoManager();

    void init();
    void convertSymmetricKeyToHex();


    void printHExSymmetricKey();
};

