//
// Created by Stoorx on 18.07.2019.
//

#include "FileManager.hpp"
#include <Commands/CopyFileCommand.hpp>

FileManager::FileManager(Context& context) : ImageContext(context) {}

void FileManager::copyFile(const std::string& from, const std::string& to, const std::vector<bool>& attributes) {
    std::string currentPath = "";
    bool        isDir       = true;
    for (size_t i           = 1; i <= to.size(); ++i) {
        if (i != to.size() && to[i] != '/') {
            currentPath += to[i];
            if (to[i] == '.') {
                isDir = false;
            }
        } // FIXME!!!: it does not work
        else {
            auto        dir      = opendir(currentPath.c_str()); // FIXME!!!: it does not work
            std::string FSNumber = "";
            for (int    c        = 3;; ++c) {
                if (isdigit(to[c])) {
                    FSNumber += to[c];
                }
                else {
                    break;
                }
            } // TODO: make this code better (maybe with STL methods)
            
            if (!dir) {
                if (isDir) {
                    this->ImageContext.FilesystemManagers[std::stoll(FSNumber)]
                        ->createFile(currentPath, CreateFileAttributes::Directory);
                }
                else {
                    auto file = this->ImageContext.FilesystemManagers[std::stoll(FSNumber)]
                        ->createFile(currentPath, CreateFileAttributes::File);
                    dataTransfer(from, file.get());
                }
            }
            else {
                if (!isDir) {
                    if (attributes[static_cast<int>(CopyFileAttributes::Force)]) {
                        auto file = this->ImageContext.FilesystemManagers[std::stoll(FSNumber)]
                            ->createFile(currentPath, CreateFileAttributes::File);
                        dataTransfer(from, file.get());
                    }
                    else {
                        throw; //TODO: throw something
                    }
                }
            }
        }
        currentPath += to[i];
    }
}