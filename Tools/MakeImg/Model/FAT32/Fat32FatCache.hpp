//
// Created by Stoorx on 13.07.2019.
//

#pragma once

#include <stdint.h>
#include <Model/Context.hpp>
#include <vector>

class Fat32FsManager;

class Fat32FatCache {
public:
    Fat32FatCache(Context& context, Fat32FsManager& fsManager, uint32_t cacheSize);
    
    uint32_t operator[](uint32_t idx);

protected:
    std::vector<uint32_t> Cache;
    uint64_t              CurrentImageOffset;
    
    Context& CurrentContext;
    
    Fat32FsManager& FsManager;
private:
    uint64_t mFatOffset;
};
