//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <string>
#include <Context.hpp>

class Command {
public:
    Command() = default;
    virtual void Execute(Context& context) = 0; // TODO: add context as argument
};

class NewImageCommand : public Command {
public:
    NewImageCommand(std::string& fileName, uint64_t fileSize) : FileName(fileName), FileSize(fileSize) {};
    
    void Execute(Context& context) override {
    
    }
    
    const std::string FileName;
    const uint64_t    FileSize = 0;
};



