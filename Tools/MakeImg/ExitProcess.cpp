//
// Created by Stoorx on 19.06.2019.
//

#include <ExitProcess.hpp>

void ExitProcess(int exitCode) {
    throw ExitProcessException(exitCode);
}
