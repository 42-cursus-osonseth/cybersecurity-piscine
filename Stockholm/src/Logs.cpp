#include "Logs.hpp"

Logs::Logs()
{
}
Logs::Logs(bool silent) : _silent(silent)
{
}

Logs::~Logs()
{
}

void Logs::setSilent(bool s)
{
    _silent = s;
}

void Logs::startEncryption() const
{
    if (_silent)
        return;
    const std::string block =
        COLOR_YELLOW
        "************************************************************\n"
        "*                                                          *\n"
        "*                     START FILES ENCRYPTION               *\n"
        "*                                                          *\n"
        "************************************************************\n" COLOR_RESET;

    std::cout << block << std::endl;
}

void Logs::startDecryption() const
{
    if (_silent)
        return;
    const std::string block =
        COLOR_YELLOW
        "************************************************************\n"
        "*                                                          *\n"
        "*                     START FILES DECRYPTION               *\n"
        "*                                                          *\n"
        "************************************************************\n" COLOR_RESET;

    std::cout << block << std::endl;
}

void Logs::fileToProcess(fs::path path)
{
    if (_silent)
        return;
    fs::path parent = path.parent_path();
    fs::path filename = path.filename();

    std::cout << COLOR_GREEN << filename.string()
              << COLOR_RESET << " in "
              << COLOR_CYAN << parent.string()
              << COLOR_RESET << std::endl;
}

void Logs::successfullyEncrypted(fs::path path)
{
    if (_silent)
        return;
    fs::path filename = path.filename();
    std::cout << COLOR_GREEN << filename.string()
              << COLOR_RED << "  Encrypted Successfully !"
              << COLOR_RESET << std::endl
              << std::endl;
}

void Logs::successfullyDecrypted(fs::path path)
{
    if (_silent)
        return;
    fs::path filename = path.filename();
    std::cout << COLOR_GREEN << filename.string()
              << COLOR_RED << "  Decrypted Successfully !"
              << COLOR_RESET << std::endl
              << std::endl;
}
