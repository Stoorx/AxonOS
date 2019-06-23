//
// Created by Stoorx on 23.06.2019.
//

#pragma once
#include "Command.hpp"

class LoadImageCommand : public Command {
public:
    explicit LoadImageCommand(const std::string& fileName);
    
    void Execute(Context& context) override;
    
    const std::string FileName;
};

