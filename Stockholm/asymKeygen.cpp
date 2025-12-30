#include <sodium.h>
#include <iostream>


#include <string.h>

constexpr auto SIGN_PUB_SIZE = crypto_sign_ed25519_PUBLICKEYBYTES;
constexpr auto SIGN_PRIV_SIZE = crypto_sign_ed25519_SECRETKEYBYTES;
constexpr auto ENC_PUB_SIZE = crypto_box_PUBLICKEYBYTES;
constexpr auto ENC_PRIV_SIZE = crypto_box_SECRETKEYBYTES;
constexpr auto HEX_KEY_SIZE = ENC_PRIV_SIZE * 2 + 1;

int main(void){

    try{
    if (sodium_init() < 0)
        throw(std::runtime_error("Lipsodium initialization failed"));
    unsigned char sign_public [SIGN_PUB_SIZE];
    unsigned char sign_priv [SIGN_PRIV_SIZE];
    unsigned char encrypt_public [ENC_PUB_SIZE];
    unsigned char encrypt_priv[ENC_PRIV_SIZE];
    char binKeytoHex[HEX_KEY_SIZE];

    crypto_sign_ed25519_keypair(sign_public, sign_priv);

    if (crypto_sign_ed25519_pk_to_curve25519(encrypt_public, sign_public) != 0)
        throw std::runtime_error("Failed to convert public key");

    if (crypto_sign_ed25519_sk_to_curve25519(encrypt_priv, sign_priv) != 0)
        throw std::runtime_error("Failed to convert secret key");
    
    sodium_bin2hex(binKeytoHex, HEX_KEY_SIZE, encrypt_public, ENC_PUB_SIZE);
    std::cout << "key public  : " << binKeytoHex << std::endl;
    std::cout << "len = " << strlen(binKeytoHex) << std::endl;
    sodium_bin2hex(binKeytoHex, HEX_KEY_SIZE, encrypt_priv, ENC_PRIV_SIZE);
    std::cout << "key private : " << binKeytoHex << std::endl;
    std::cout << "len = " << strlen(binKeytoHex) << std::endl;

    }
    catch (const std::exception &e){
        std::cout << e.what() << std::endl;
        std::exit(1);
    }
    return 0;  
}