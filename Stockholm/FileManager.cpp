#include "FileManager.hpp"

namespace fs = std::filesystem;

FileManager::FileManager() : _targetFolder("Infection")
{
    const char* home = std::getenv("HOME");
    if (!home)
        throw std::runtime_error("HOME environment variable not set");
    bool found = false;

    for (const auto& entry : fs::directory_iterator(home)) {
        if (entry.is_directory() && entry.path().filename() == _targetFolder) {
            found = true;
            break;
        }
    }

    if (!found)
        throw std::runtime_error("Infection directory not found in the user's HOME");


}

FileManager::~FileManager()
{
}