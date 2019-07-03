//
// Created by Stoorx on 23.06.2019.
//

#include "NewTableMbrCommand.hpp"
#include <Model/PartitionTableManager.hpp>
#include <memory.h>

NewTableMbrCommand::NewTableMbrCommand(const std::string& mbrFileName) :
    UseDefaultBootloader(false), MbrFileName(mbrFileName) {
}

void NewTableMbrCommand::Execute(Context& context) {
    context.PartitionManager = std::shared_ptr<PartitionTableManager>(new MbrPartitionTableManager());
    
    auto params = MbrCreatePartitionTableParameters();
    params.BootsectorFileName   = MbrFileName;
    params.UseDefaultBootloader = UseDefaultBootloader;
    
    context.PartitionManager->CreatePartitionTable(context, params);
}
