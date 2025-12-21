#include "FileManager.hpp"

FileManager::FileManager()
{
    const char * home = std::getenv("HOME");
    if (!home)
        throw std::runtime_error("HOME environment variable not set");
    
    fs::path homePath(home);
    _targetFolder = homePath / "Infection";

    if (!fs::exists(_targetFolder) || !fs::is_directory(_targetFolder))
        throw std::runtime_error("Infection directory not found in the user's HOME");


}
std::vector<fs::path> FileManager::getFilesToEncrypt(){

    std::vector<fs::path> files;
    for (const auto& entry : fs::recursive_directory_iterator(_targetFolder, fs::directory_options::skip_permission_denied)) {
    if (entry.is_regular_file()) {
        files.push_back(entry.path());
        }
    }
    return files;
}

FileManager::~FileManager()
{
}