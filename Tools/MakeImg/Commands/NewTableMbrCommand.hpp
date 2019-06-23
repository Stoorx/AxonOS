//
// Created by Stoorx on 23.06.2019.
//

#pragma once

#include "Command.hpp"
#include <filesystem>
#include <Exceptions/FileNotFoundException.hpp>

class NewTableMbrCommand : public Command {
public:
    NewTableMbrCommand() : UseDefaultBootloader(true) {
    
    }
    
    explicit NewTableMbrCommand(const std::string& mbrFileName) :
        UseDefaultBootloader(false), MbrFileName(mbrFileName) {
    }
    
    void Execute(Context& context) override {
        if(std::filesystem::exists(MbrFileName)){
            // TODO: logic
        }else{
            throw FileNotFoundException(MbrFileName);
        }
    }
    
    const bool        UseDefaultBootloader;
    const std::string MbrFileName;
};


