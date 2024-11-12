//
// Created by neoro on 12/05/2024.
//

#include "../../include/Utilities/Utilities.h"
#include <fstream>
#include <sstream>

namespace DaedalusEngine {
    std::vector<char> ReadFileAsBinary(const std::string& fileName) {
        std::ifstream fileHandle(fileName, std::ios::ate | std::ios::binary);

        size_t fileSize = (size_t)fileHandle.tellg();
        std::vector<char> rawFileData(fileSize);

        fileHandle.seekg(0);
        fileHandle.read(rawFileData.data(), fileSize);
        fileHandle.close();

        return rawFileData;
    }
}