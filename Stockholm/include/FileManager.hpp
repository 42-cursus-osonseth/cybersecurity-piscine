#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;

class FileManager
{
private:
fs::path _targetFolder;
bool isValidExtension (std::string) const;
    
public:
    FileManager();
    ~FileManager();

    std::vector<fs::path> getTargetFiles() const;
    std::ifstream openOriginalFile(fs::path path) const;
    std::ofstream createNewFile(fs::path path, fs::path &tmpPath) const;
    void addFtExt(fs::path &path) const;
    void removeFtExt(fs::path &path) const;
    
};


