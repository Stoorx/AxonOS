//
// Created by Stoorx on 23.06.2019.
//

#include "CommandSequence.hpp"

void CommandSequence::Append(const std::shared_ptr<Command>& command) {
    commandsList.push_back(command);
}

void CommandSequence::Execute(Context& context) {
    for (auto& command: commandsList){
        command->Execute(context);
    }
}
