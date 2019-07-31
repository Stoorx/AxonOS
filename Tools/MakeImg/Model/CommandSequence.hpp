//
// Created by Stoorx on 23.06.2019.
//

#pragma once

#include <vector>
#include <Commands/Command.hpp>
#include <memory>
#include <Model/Context.hpp>

class CommandSequence {
public:
    void Execute(Context& context);
    void Append(const std::shared_ptr<Command>& command);
protected:
    std::vector<std::shared_ptr<Command>> commandsList;
};
