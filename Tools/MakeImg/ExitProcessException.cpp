//
// Created by Stoorx on 19.06.2019.
//
#include <ExitProcessException.hpp>

const char* ExitProcessException::what() const noexcept {
    return "exit";
}

ExitProcessException::ExitProcessException(int exitCode) : std::exception(), ExitCode(exitCode) {}
