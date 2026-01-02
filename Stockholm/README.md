# Stockholm

Encrypts and decrypts files using XChaCha20-Poly1305 with asymmetric key exchange.

The program only encrypts files located in the user's Infection folder in the home directory.

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
    ```bash
    Make
    ```
## Usage

You can generate a symmetric key pair by creating the keygen program and running it.
    ```bash
    Make key
    ./keygen
    ```

