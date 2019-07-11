//
// Created by Stoorx on 27.06.2019.
//

#pragma once

#include "Command.hpp"
#include <Model/PartitionTableManager.hpp>
#include <vector>
#include <string>
#include <set>

class NewPartCommand : public Command {
public:
    NewPartCommand(
        int partitionNumber,
        uint64_t begin,
        uint64_t end,
        const std::set<std::string>& params
    ) : PartitionNumber(partitionNumber), Begin(begin), End(end), Params(params) {}
    
    void Execute(Context& context) override {
        auto partitionType = context.PartitionManager->GetPartitionTableType();
    
        switch (partitionType) {
            case PartitionTableType::MBR: {
                MbrRegisterPartitionParameters params{};
                params.Active                  = Params.find("active") != Params.end();
                params.StartHead               = 0; // TODO: CHS support, add geometry parameters into disk image
                params.StartSectorCylinderHigh = 0;
                params.StartCylinderLow        = 0;
                params.PartitionType           = 0; // because we don't know the FS type yet
                params.EndHead                 = 0;
                params.EndSectorCylinderHigh   = 0;
                params.EndCylinderLow          = 0;
                params.StartLBA                = Begin;
                params.EndLBA                  = End;
            
                context.PartitionManager->RegisterPartition(context, PartitionNumber, params);
            }
                break;
            case PartitionTableType::GPT:
                throw std::exception(); // TODO: replace with NotImplementedException with message
            default:
                throw std::exception(); // TODO: replace with InvalidParameterError with message
        }
    }

public:
    const int                   PartitionNumber;
    const uint64_t              Begin;
    const uint64_t              End;
    const std::set<std::string> Params;
};

