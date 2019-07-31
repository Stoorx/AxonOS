//
// Created by Stoorx on 19.06.2019.
//

#pragma once

#include <string>
#include <vector>
#include <Util/ExitProcess.hpp>
#include <iostream>
#include <Exceptions/CommandParseException.hpp>
#include <Commands/Command.hpp>
#include <Commands/NewImageCommand.hpp>
#include <Commands/LoadImageCommand.hpp>
#include <Commands/NewTableMbrCommand.hpp>
#include <Model/CommandSequence.hpp>
#include <Commands/NewPartCommand.hpp>
#include <Util/StringUtil.hpp>
#include <memory>

using namespace std;

class MakeImg {
public:
    static void Main(vector<string>& args);
    
    static shared_ptr<Command> ParseNewImage(const vector<string>& inputTokens, uint64_t& inputPosition);
    static shared_ptr<Command> ParseLoadImage(const vector<string>& inputTokens, uint64_t& inputPosition);
    static shared_ptr<Command> ParseNewTable(const vector<string>& inputTokens, uint64_t& inputPosition);
    static shared_ptr<Command> ParseNewPart(const vector<string>& inputTokens, uint64_t& inputPosition);
    static shared_ptr<Command> ParseNewFs(const vector<string>& inputTokens, uint64_t& inputPosition);
    static shared_ptr<Command> ParseCopyFile(const vector<string>& inputTokens, uint64_t& inputPosition);
    
    static void ExitWithUsage();
    
    static void ExitWithError(const string& message, int exitCode);
};



