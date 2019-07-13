//
// Created by Stoorx on 13.07.2019.
//

#pragma once

#include <stdint.h>
#include <Model/Context.hpp>
#include "Fat32FsManager.hpp"
#include <vector>

class Fat32FatCache {
public:
    Fat32FatCache(Context& context, Fat32FsManager& fsManager, uint32_t cacheSize) :
        Context(context),
        FsManager(fsManager),
        CurrentImageOffset(0),
        Cache(cacheSize, 0),
        mFatOffset(fsManager.GetFatOffset()) {
        
    }
    
    uint32_t operator[](uint32_t idx) {
        // TODO: add logic
    }

protected:
    std::vector<uint32_t> Cache;
    uint64_t              CurrentImageOffset;
    
    Context& Context;
    
    Fat32FsManager& FsManager;
private:
    uint64_t mFatOffset;
};
