//
// Created by Stoorx on 30.07.2019.
//

#include "NewTableLoopCommand.hpp"

void NewTableLoopCommand::Execute(Context& context) {
    context.PartitionManager = std::shared_ptr<PartitionTableManager>(new LoopPartitionManager());
}
