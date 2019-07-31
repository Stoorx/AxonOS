//
// Created by cognitio on 12.07.2019.
//

#include "IncorrectParameterException.hpp"

IncorrectParameterException::IncorrectParameterException(
    const std::string& name,
    const std::string& value,
    const std::string& reason
) :
    ParameterName(name),
    ParameterValue(value),
    Reason(reason) {
    errorString = (
        "IncorrectParameterException of " +
        ParameterName +
        ",\n"
        "                         value: '" + ParameterValue +
        "',\n"
        "                         reason: '" + Reason + "'\n"
    );
}

const char* IncorrectParameterException::what() const noexcept {
    return errorString.c_str();
}

