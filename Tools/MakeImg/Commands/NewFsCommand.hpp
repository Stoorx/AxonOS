//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Commands/Command.hpp>
#include <map>
#include <Model/FAT32/Fat32FsManager.hpp>

class NewFsCommand : public Command {
public:
    NewFsCommand(
        const std::string& fsType,
        const uint32_t partitionNumber,
        const std::map<std::string, std::string>& params
    ) : FsType(fsType), PartitionNumber(partitionNumber), Params(params) {}
    
    static std::shared_ptr<Command> CreateFsCommandByType(
        const std::string& fsType,
        uint32_t partitionNumber,
        const std::map<std::string, std::string>& params
    );

protected:
    const std::string                        FsType;
    const uint32_t                           PartitionNumber;
    const std::map<std::string, std::string> Params;
};


class NewFat32FsCommand : public NewFsCommand {
public:
    NewFat32FsCommand(
        const std::string& fsType,
        const uint32_t partitionNumber,
        const std::map<std::string, std::string>& params
    ) : NewFsCommand(fsType, partitionNumber, params) {}
    
    void Execute(Context& context) override;
};
