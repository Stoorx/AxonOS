//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Commands/Command.hpp>
#include <map>

class NewFsCommand : public Command {
public:
    NewFsCommand(
        const std::string& fsType,
        const uint32_t partitionNumber,
        const std::map<std::string, std::string>& params
    ) : FsType(fsType), PartitionNumber(partitionNumber), Params(params) {}
    
    void Execute(Context& context) override {
        // TODO: Add logic
    }

protected:
    const std::string                        FsType;
    const uint32_t                           PartitionNumber;
    const std::map<std::string, std::string> Params;
};


