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

std::vector<fs::path> FileManager::getTargetFiles() const{

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

std::ofstream FileManager::createNewFile(fs::path path, fs::path &tmpPath) const{
    tmpPath = path;
    tmpPath += TMP_EXT;
    return std::ofstream (tmpPath, BIN);  
}

std::ifstream FileManager::openOriginalFile(const fs::path path) const{
     return std::ifstream(path, BIN);
}


void FileManager::addFtExt(fs::path &path) const {
    fs::path parent = path.parent_path();
    fs::path newPath= parent /path.stem();
  
    if(newPath.extension() != ".ft")
        newPath += ".ft";
    fs::rename(path, newPath);
}

void FileManager::removeFtExt(fs::path &path) const{
    fs::path parent = path.parent_path();
    fs::path filename = path.filename();
    for(int i = 0; i < 2; i++)
        filename.replace_extension();
    if(filename.extension().empty())
        filename.replace_extension(".ft");
    fs::path newPath = parent / filename;
    fs::rename(path, newPath);

}

