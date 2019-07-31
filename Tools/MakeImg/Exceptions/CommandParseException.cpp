//
// Created by Stoorx on 23.06.2019.
//

#include "CommandParseException.hpp"

CommandParseException::CommandParseException(int position, const std::string& token, const std::string& reason) :
    Reason(reason), TokenPosition(position), Token(token) {
    errorString = (
        "CommandParseException at position: " +
        std::to_string(TokenPosition) +
        ",\n"
        "                         token: '" + Token +
        "',\n"
        "                         reason: '" + Reason + "'\n"
    );
    
}

const char* CommandParseException::what() const noexcept {
    return errorString.c_str();
}
