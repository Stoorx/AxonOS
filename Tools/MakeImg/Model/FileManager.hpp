//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <Model/Context.hpp>
#include <vector>

class FileManager {
public:
    FileManager(Context& context) : ImageContext(context) {}
    // TODO: add methods
    void copyFile(const std::string& from, const std::string& to, const std::vector<bool>& attributes) {}

protected:
    Context& ImageContext;
};



