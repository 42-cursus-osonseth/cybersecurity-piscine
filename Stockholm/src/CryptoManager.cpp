#include "CryptoManager.hpp"
#include "const.hpp"
#include <iostream>

cryptoManager::cryptoManager()
{
}
cryptoManager::cryptoManager(Config conf, std::vector<fs::path> files) : _conf(conf), _files(files) {

 
}

cryptoManager::~cryptoManager()
{
}

void cryptoManager::init(){
    if (sodium_init() < 0)
        throw(std::runtime_error("Lipsodium initialization failed"));
   
    crypto_secretbox_keygen(_symmetricKey);
    convertSymmetricKeyToHex();
    printHExSymmetricKey();
}

//------------------------------------------------------------------------

void cryptoManager::convertSymmetricKeyToHex(){
    sodium_bin2hex(_hex_symmetricKey, sizeof(_hex_symmetricKey), _symmetricKey, SYMMETRIC_KEY_SIZE);
}

void cryptoManager::printHExSymmetricKey(){
     std::cout << "Key = " << _hex_symmetricKey << std::endl;
}