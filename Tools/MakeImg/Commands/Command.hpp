//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <string>
#include <Model/Context.hpp>
#include <filesystem>
#include <Exceptions/FileNotFoundException.hpp>

class Command { // interface
public:
    Command() = default;
    virtual void Execute(Context& context) = 0;
};






