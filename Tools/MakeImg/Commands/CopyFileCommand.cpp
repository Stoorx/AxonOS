//
// Created by Stoorx on 18.07.2019.
//

#include "CopyFileCommand.hpp"
#include <Model/FileManager.hpp>

CopyFileCommand::CopyFileCommand(const std::string& from, const std::string& to, const std::vector<bool>& attributes) :
    From(from), To(to), CopyAttributes(attributes) {}

void CopyFileCommand::Execute(Context& context) {
    struct stat fromPathStat = {};
    stat(From.c_str(), &fromPathStat);
    if (S_ISDIR(fromPathStat.st_mode)) {
        if (!CopyAttributes[(uint32_t)CopyFileAttributes::Recursive]) {
            throw IncorrectParameterException(
                "Recursive",
                "false",
                "Source is directory. Use -r for recursive copy"
            );
        }
        else {
            RecursiveWalk(
                From, [&](const std::string& currentFile) {
                    std::string toPath = CreateRelativePath(To, From, currentFile);
                    context.FileManager.copyFile(currentFile, toPath, CopyAttributes);
                }
            );
        }
    }
    else {
        // if it is a file
        context.FileManager.copyFile(From, To, CopyAttributes);
    }
}