//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <functional>
#include "Command.hpp"
#include <sys/stat.h>
#include <Exceptions/IncorrectParameterException.hpp>
#include <dirent.h>
#include <Model/FileManager.hpp>

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
        struct stat fromPathStat = {};
        stat(From.c_str(), &fromPathStat);
        if (S_ISDIR(fromPathStat.st_mode)) {
            if (!CopyAttributes[(uint32_t)CopyFileAttributes::Recursive])
                throw IncorrectParameterException(
                    "Recursive",
                    "false",
                    "Source is directory. Use -r for recursive copy"
                );
            else {
                RecursiveWalk(
                    From, [&](const std::string& currentFile) {
                        std::string toPath = ""; //TODO: make a relatve path from currentFile path
                        context.FileManager.copyFile(currentFile, toPath, CopyAttributes);
                    }
                );
            }
        }
        else {
            // if it is file
            std::string toPath = ""; //TODO: make a relatve path from currentFile path
            context.FileManager.copyFile(From, toPath, CopyAttributes);
        }
    }

protected:
    const std::string       From;
    const std::string       To;
    const std::vector<bool> CopyAttributes;
private:
    static void RecursiveWalk(const std::string& path, const std::function<void(const std::string&)>& cb) {
        // TODO: test & debug
        if (auto dir = opendir(path.c_str())) {
            while (auto f = readdir(dir)) {
                if (!f->d_name || f->d_name[0] == '.') continue;
                struct stat dirStat = {};
                stat(f->d_name, &dirStat);
                if (S_ISDIR(dirStat.st_mode))
                    RecursiveWalk(path + f->d_name + "/", cb);
                
                if (S_ISREG(dirStat.st_mode))
                    cb(path + f->d_name);
            }
            closedir(dir);
        }
    }
};



