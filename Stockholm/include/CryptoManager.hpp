#pragma once


#include <vector>
#include <sodium.h>
#include <array>
#include <cstdint>
#include "FileManager.hpp"
#include "type.hpp"
#include "const.hpp"
#include "Logs.hpp"

namespace fs = std::filesystem;

class cryptoManager
{
private:
    Config &_conf;
    FileManager _filemanager;
    Logs &_logs;

    std::vector<fs::path> _files;

    fs::path _tmpPath;

    std::ifstream _in;
    std::ofstream _out;

    std::array<unsigned char, SYMMETRIC_KEY_SIZE> _symKey{};
    std::array<char, HEX_SYMMETRIC_KEY_SIZE> _hex_symKey{};
    std::array<unsigned char, ENCRYPTED_SYM_KEY_SIZE> _symKeyEncrypted{};
    std::array<unsigned char, ASYM_KEY_SIZE> _asymKeyPub{};
    std::array<unsigned char, ASYM_KEY_SIZE> _asymKeyPriv{}; 

    std::vector<unsigned char> _buffer{};
    std::vector<unsigned char> _encryptedBuffer{};
    std::vector<unsigned char> _header{};

    crypto_secretstream_xchacha20poly1305_state _state;

    void encryptFiles();
    void decryptFiles();

    void openFileStreams(fs::path file);
    void generateSymKey();
    void encryptSymmKey();
    void writeEncryptSymmKey();
    void initSecretStreamPush();
    void writeHeader();
    void writeEncryptedData();
    void cryptAndWriteData(std::streamsize bytesRead);
    void addFinalTag();

    void readEncryptedSymKey();
    bool decryptSymKey();
    void readSecretStreamHeader();
    bool initSecretStreamPull();
    void writeDecryptedData();
    void readCryptedData();
    bool decryptData(unsigned char &tag, unsigned long long &out_len, std::streamsize bytesRead);

public:
    cryptoManager(Config &conf, Logs &logs);
    ~cryptoManager();

    void init();
    void processFiles();


    void convertsymKeyToHex();
    void hexToBinAsymPubKey();
    void printHExsymKey();

    void hexToBinAsymPrivKey();
};
