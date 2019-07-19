//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <Model/DiskImage.hpp>
#include <Model/FsManager.hpp>
#include <memory>
#include <map>
#include <Model/FileManager.hpp>

class PartitionTableManager;

class Context {
public:
    Context() : FileManager(*this) {}
    
    std::shared_ptr<DiskImage>                     DiskImage        = nullptr;
    std::shared_ptr<PartitionTableManager>         PartitionManager = nullptr;
    std::map<uint64_t, std::shared_ptr<FsManager>> FilesystemManagers;
    FileManager                                    FileManager;
};
