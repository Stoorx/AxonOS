//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include "File.hpp"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <math.h>

class Context;

class FileManager {
public:
    FileManager(Context& context);
    // TODO: add methods
    void copyFile(const std::string& from, const std::string& to, const std::vector<bool>& attributes);

protected:
    Context& ImageContext;
    
    void static dataTransfer(const std::string& from, File* to) {
        std::ifstream fromFile(from, std::ios::in | std::ios::binary);
        if (!fromFile || !fromFile.is_open()) {
            throw; //TODO: throw something
        }
        
        struct stat fromResult = {};
        uint64_t    fromSize;
        if (stat(from.c_str(), &fromResult) == 0) {
            fromSize = (uint64_t)fromResult.st_size; // unawares _off_t is signed
        }
        else {
            throw; //TODO: throw something
        }
        
        uint64_t offset = 0;
        char* buffer = new char[(uint64_t)std::sqrt(fromSize)];
        while (offset != fromSize) {
            uint64_t bufSize = std::min(fromSize - offset, (uint64_t)std::sqrt(fromSize));
            fromFile.read(buffer, bufSize);
            auto getSize = (uint64_t)fromFile.gcount();
            if (!fromFile || !getSize || (getSize != bufSize)) //TODO: make it clear; i think it is surplus
            {
                throw; //TODO: throw something
            }
            to->write(buffer, offset, getSize);
            offset += getSize;
        }
        delete[] buffer;
    
        fromFile.close();
    }
};



