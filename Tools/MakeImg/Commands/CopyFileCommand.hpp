//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <functional>
#include "Command.hpp"


enum class CopyFileAttributes {
    Recursive,
    Force,
    PreserveTimestamps
};

class CopyFileCommand : public Command {
public:
    CopyFileCommand(const std::string& from, const std::string& to, const std::vector<bool>& attributes) :
        From(from), To(to), CopyAttributes(attributes) {}
    
    void Execute(Context& context) override {
        // TODO: implement
    }

protected:
    const std::string       From;
    const std::string       To;
    const std::vector<bool> CopyAttributes;
private:

};



