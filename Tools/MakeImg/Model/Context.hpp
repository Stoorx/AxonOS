//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <Model/DiskImage.hpp>
#include <memory>

class PartitionTableManager;

class Context {
public:
    std::shared_ptr<DiskImage>             DiskImage        = nullptr;
    std::shared_ptr<PartitionTableManager> PartitionManager = nullptr;
};
