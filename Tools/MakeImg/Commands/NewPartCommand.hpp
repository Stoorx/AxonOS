//
// Created by Stoorx on 27.06.2019.
//

#pragma once

#include "Command.hpp"
#include <vector>
#include <string>
#include <map>

class NewPartCommand : public Command {
public:
    NewPartCommand(
        int partitionNumber,
        uint64_t begin,
        uint64_t end,
        const std::map<std::string, std::string>& params
    ) : PartitionNumber(partitionNumber), Begin(begin), End(end), Params(params) {}
    
    void Execute(Context& context) override {
    
    }

public:
    const int      PartitionNumber;
    const uint64_t Begin;
    const uint64_t End;
    const std::map<std::string, std::string>& Params;
};

