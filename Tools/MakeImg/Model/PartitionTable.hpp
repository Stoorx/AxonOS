//
// Created by Stoorx on 27.06.2019.
//

#pragma once

#include <stdint.h>
#include <bits/shared_ptr.h>
#include "Context.hpp"

enum class PartitionTableType {
    NotSet,
    Invalid,
    MBR,
    GPT
};

class PartitionParameters {
public:
    const uint32_t     Number;
    const uint64_t     Begin;
    const uint64_t     End;
    PartitionTableType Type;
    virtual ~PartitionParameters() = default;
};

class MbrPartitionParameters : public PartitionParameters {
public:
    const bool Active;
};

class PartitionTable {
public:
    const PartitionTableType PartitionType = PartitionTableType::NotSet;
    
    virtual void RegisterPartition(Context& context, const PartitionParameters& parameters) = 0;
};

class MbrPartitionTable : PartitionTable {
    void RegisterPartition(Context& context, const PartitionParameters& parameters) override {
        auto params = dynamic_cast<const MbrPartitionParameters*>(&parameters);
        if (params != nullptr) {
            // TODO: add logic
        }
    }
};
