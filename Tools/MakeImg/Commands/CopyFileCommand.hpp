//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <functional>
#include "Command.hpp"
#include <sys/stat.h>
#include <Exceptions/IncorrectParameterException.hpp>
#include <dirent.h>

enum class CopyFileAttributes {
    Recursive,
    Force,
    PreserveTimestamps
};

class CopyFileCommand : public Command {
public:
    CopyFileCommand(const std::string& from, const std::string& to, const std::vector<bool>& attributes);
    void Execute(Context& context) override;

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
    
    static std::string CreateRelativePath(const std::string& to, const std::string& from, const std::string& current) {
        std::string add    = current.substr(from.size() + 2);
        std::string result = (to[0] == '/' ? "" : "/") + to + add;
        for (auto& c : result) {
            if (c == '\\') {
                c = '/';
            }
        }
        
        return result;
    }
};



