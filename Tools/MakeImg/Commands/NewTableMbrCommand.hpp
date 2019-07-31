//
// Created by Stoorx on 23.06.2019.
//

#pragma once

#include "Command.hpp"
#include <Exceptions/FileNotFoundException.hpp>
#include <Model/DiskImage.hpp>
#include <Model/Context.hpp>
#include <fstream>

class NewTableMbrCommand : public Command {
public:
    NewTableMbrCommand() : UseDefaultBootloader(true) {
    
    }
    
    explicit NewTableMbrCommand(const std::string& mbrFileName);
    
    void Execute(Context& context) override;
    
    const bool        UseDefaultBootloader;
    const std::string MbrFileName;
};


