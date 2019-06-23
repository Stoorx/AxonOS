//
// Created by Stoorx on 23.06.2019.
//

#pragma once
#include "Command.hpp"
#include <Exceptions/IllegalStateException.hpp>

class NewImageCommand : public Command {
public:
    NewImageCommand(std::string& fileName, uint64_t fileSize);;
    
    void Execute(Context& context) override;
    
    const std::string FileName;
    const uint64_t    FileSize = 0;
};
