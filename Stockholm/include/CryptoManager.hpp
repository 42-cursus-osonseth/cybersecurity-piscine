#pragma once

#include "type.hpp"
#include <vector>
#include <FileManager.hpp>
#include <sodium.h>
#include <const.hpp>
#include <array>
#include <cstdint>

namespace fs = std::filesystem;

class cryptoManager
{
private:
    Config _conf;
    std::vector<fs::path> _files;

    std::array<unsigned char, SYMMETRIC_KEY_SIZE> _symKey{};
    std::array<char, HEX_SYMMETRIC_KEY_SIZE> _hex_symKey{};
    std::array<unsigned char, ENCRYPTED_SYM_KEY_SIZE> _symKeyEncrypted{};
    std::array<unsigned char, ASYM_KEY_SIZE> _asymKey{};

    std::vector<unsigned char> _buffer{};
    std::vector<unsigned char> _encryptedBuffer{};
    std::vector<unsigned char> _header{};

    crypto_secretstream_xchacha20poly1305_state _state;

    void generateAsymKey();
    void encryptSymmKey();
    void writeEncryptSymmKey(std::ofstream &out) const;
    void writeHeader(std::ofstream &out) const;
    void writeEncryptedData(std::ifstream &in, std::ofstream &out);

public:
    cryptoManager();
    cryptoManager(Config conf, std::vector<fs::path> files);
    ~cryptoManager();

    void init();
    void convertsymKeyToHex();
    void hexToBinAsymPubKey();
    void printHExsymKey();
};
