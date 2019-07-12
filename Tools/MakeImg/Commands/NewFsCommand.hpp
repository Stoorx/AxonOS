//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Commands/Command.hpp>
#include <map>

class NewFsCommand : public Command {
public:
    NewFsCommand(
        const std::string& fsType,
        const std::map<std::string, std::string>& params
    ) : FsType(fsType), Params(params) {}
    
    void Execute(Context& context) override {
        // TODO: Add logic
    }

protected:
    std::string                        FsType;
    std::map<std::string, std::string> Params;
};


