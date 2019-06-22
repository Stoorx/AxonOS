//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <string>
#include <Context.hpp>

class Command {
public:
    Command() = default;
    virtual void Execute(Context& context) = 0;
};

class IllegalStateException : std::exception {
public:
    explicit IllegalStateException(const std::string& msg) : std::exception() {
        message = msg;
    }

private:
    const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

class NewImageCommand : public Command {
public:
    NewImageCommand(std::string& fileName, uint64_t fileSize) : Command(), FileName(fileName), FileSize(fileSize) {};
    
    void Execute(Context& context) override {
        if (context.DiskImage != nullptr) {
            throw IllegalStateException("Attempt of creating new image while active image exists");
        }
        context.DiskImage = new DiskImage(DiskImage::CreateEmptyDiskImage(FileName, FileSize));
    }
    
    const std::string FileName;
    const uint64_t    FileSize = 0;
};



