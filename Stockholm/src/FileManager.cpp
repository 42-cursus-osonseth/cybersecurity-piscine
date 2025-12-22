#include "FileManager.hpp"
#include "const.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

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

FileManager::~FileManager()
{
}

std::vector<fs::path> FileManager::getFilesToEncrypt() const{

    std::vector<fs::path> files;
    for (const auto& entry : fs::recursive_directory_iterator(_targetFolder, fs::directory_options::skip_permission_denied)) {
    if (entry.is_regular_file() && isValidExtension(entry.path().extension().string())) {
        files.push_back(entry.path());
        }
    }
    return files;
}

bool FileManager::isValidExtension(std::string path) const {
    boost::algorithm::to_lower(path);
    return std::any_of(VALID_EXT.begin(), VALID_EXT.end(), [&](const std::string& ext){ return path == ext; });
}
