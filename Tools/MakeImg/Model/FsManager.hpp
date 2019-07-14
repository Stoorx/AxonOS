//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <string>
#include <algorithm>

class Context;

class FsFormatPartitionParameters {
public:
    virtual ~FsFormatPartitionParameters() = default;
};

class FsManager { // abstract
public:
    explicit FsManager(Context& context) : ImageContext(context) {}
    
    virtual void FormatPartition(const FsFormatPartitionParameters& params) = 0;
    // TODO: add files operations
protected:
    Context& ImageContext;
};

