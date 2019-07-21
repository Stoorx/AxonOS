//
// Created by Stoorx on 18.07.2019.
//

#include "FileManager.hpp"
#include <Commands/CopyFileCommand.hpp>
#include <Util/StringUtil.hpp>

FileManager::FileManager(Context& context) : ImageContext(context) {}

void FileManager::copyFile(const std::string& from, const std::string& to, const std::vector<bool>& attributes) {
    std::string currentPath = "";
    for (size_t i           = 1; i <= to.size(); ++i) {
        if (i == to.size() || to[i] == '/') {
            std::string FSNumber = "";
            for (int    c        = 3;; ++c) {
                if (isdigit(to[c])) {
                    FSNumber += to[c];
                }
                else {
                    break;
                }
            } // TODO: make this code better (maybe with STL methods)
            
            auto dir = this->ImageContext.FilesystemManagers[std::stoll(FSNumber)]
                ->openFile(currentPath, OpenFileAttributes::Default);
            if (!dir.get()) {
                if (dir->isDirectory()) {
                    this->ImageContext.FilesystemManagers[StringUtil::ToLong(FSNumber)]
                        ->createFile(currentPath, CreateFileAttributes::Directory);
                }
                else {
                    auto file = this->ImageContext.FilesystemManagers[StringUtil::ToLong(FSNumber)]
                        ->createFile(currentPath, CreateFileAttributes::File);
                    dataTransfer(from, file.get());
                }
            }
            else {
                if (dir->isDirectory()) {
                    if (attributes[static_cast<int>(CopyFileAttributes::Force)]) {
                        this->ImageContext.FilesystemManagers[StringUtil::ToLong(FSNumber)]
                            ->deleteFile(currentPath);
                        auto file = this->ImageContext.FilesystemManagers[StringUtil::ToLong(FSNumber)]
                            ->createFile(currentPath, CreateFileAttributes::File);
                        dataTransfer(from, file.get());
                    }
                    else {
                        throw; //TODO: throw something
                    }
                }
            }
        }
        
        if (i != to.size()) {
            currentPath += to[i];
        }
    }
}