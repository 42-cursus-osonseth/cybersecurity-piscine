#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

class Logs
{
private:
    bool _silent;
    
public:
    Logs();
    Logs(bool silent);
    ~Logs();

    void setSilent(bool s);

    void startEncryption() const;
    void startDecryption() const;
    void fileToProcess(fs::path  path);
    void successfullyEncrypted (fs::path  path);
    void successfullyDecrypted (fs::path  path);
};


