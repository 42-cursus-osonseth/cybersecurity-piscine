# Stockholm

- Encrypts and decrypts files using XChaCha20-Poly1305 with asymmetric key exchange.
- The program only encrypts files located in the user's Infection folder in the home directory.
- Encryption uses the public key embedded in const.hpp.
- Decryption only works if the provided private key matches this public key.

## Prerequisites

- A modern C++ compiler (C++17 recommended, e.g., `g++ >= 11`)  

- **C++ compiler**:- A modern C++ compiler supporting **at least C++17** (e.g., `g++ >= 11`)
  
  This project was compiled with **g++ 11.4.0**, so at least **g++ >= 11** is recommended.  
  You can check your version with:
  ```bash
  c++ -v
- **libsodium library**: Used for encryption.
It can be installed via package on Ubuntu/Debian: 
    ```bash
    sudo apt install libsodium-dev 
    ```
    Or follow the official installation guide: https://libsodium.gitbook.io/doc/installation

## Compilation

Build with Make:

    Make


## Usage

You can generate a symmetric key pair by creating the keygen program and running it.


    Make key
    ./keygen


## Options

- -r, --reverse -> Decrypt files.
Requires the private key corresponding to the public key defined in const.hpp.

- -s, --silent ->
Silent mode. No output is displayed during processing.

