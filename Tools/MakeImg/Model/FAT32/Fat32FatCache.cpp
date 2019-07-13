//
// Created by Stoorx on 13.07.2019.
//

#include "Fat32FatCache.hpp"
#include "Fat32FsManager.hpp"

Fat32FatCache::Fat32FatCache(Context& context, Fat32FsManager& fsManager, uint32_t cacheSize) :
    CurrentContext(context),
    FsManager(fsManager),
    CurrentImageOffset(0),
    Cache(cacheSize, 0),
    mFatOffset(fsManager.GetFatOffset()) {
    
}

uint32_t Fat32FatCache::operator[](uint32_t idx) {
    // TODO: add logic
    return 0;
}
