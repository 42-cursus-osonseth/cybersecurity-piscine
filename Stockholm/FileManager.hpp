#pragma once

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace fs = std::filesystem;

class FileManager
{
private:
fs::path _targetFolder;
    
public:
    FileManager();
    ~FileManager();

    std::vector<fs::path> getFilesToEncrypt();
    
};


