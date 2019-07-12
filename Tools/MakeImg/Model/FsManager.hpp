//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <string>
#include <algorithm>
#include "Context.hpp"

class FsFormatPartitionParameters {
public:
    virtual ~FsFormatPartitionParameters() = default;
};

class FsManager { // interface
public:
    virtual bool CheckFsSupport(const std::string& fsName) = 0;
    virtual void FormatPartition(Context& context, uint32_t number, const FsFormatPartitionParameters& params) = 0;
    // TODO: add files operations
};

