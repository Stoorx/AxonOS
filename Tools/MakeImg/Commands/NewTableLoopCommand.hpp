//
// Created by Stoorx on 30.07.2019.
//

#pragma once

#include "Command.hpp"
#include <Model/PartitionTableManager.hpp>

class NewTableLoopCommand : public Command {
public:
    NewTableLoopCommand() = default;
    void Execute(Context& context) override;
};



