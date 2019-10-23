//
// Created by Stoorx on 19.06.2019.
//


#include <Commands/NewFsCommand.hpp>
#include <Commands/CopyFileCommand.hpp>
#include <Commands/NewTableLoopCommand.hpp>
#include "MakeImg.hpp"

void MakeImg::ExitWithError(const string& message, int exitCode) {
    cout << message << endl;
    ExitProcess(exitCode);
}

void MakeImg::ExitWithUsage() {
    cout <<
         "MakeImg - tool for creating disk images. Takes a scenario from command line and executes it step by step.\n"
         "Usage:\n"
         "    makeimg [new_image file_name -[sb|sk|sm|sg|ss] size]\n"
         "            [load_image img_file_name]\n"
         "            [new_table <mbr|gpt|loop> -b mbr_file]\n"
         "            [new_part partition_number -b begin -e end -f flags]\n"
         "            [new_fs fs_type -n part_number -p_* params]\n"
         "Commands:\n"
         "    help - show this help\n"
         "    new_image - create new image file of given size\n"
         "       -sb - in bytes\n"
         "       -sk - in kilobytes\n"
         "       -sm - in megabytes\n"
         "       -sg - in gigabytes\n"
         "       -ss - in sectors\n"
         "    load_image - take an existing file to create image inside\n"
         "    new_table - create new partition table\n"
         "       mbr - MS-DOS style partition table\n"
         "       gpt - GPT style\n"
         "       -b - load bootsector code from given file (for mbr partition table only)\n"
         "    new_part - create new partition\n"
         "       -b - begin sector of the partition\n"
         "       -e - end sector of the partition\n"
         "       -f - partition flags\n"
         "    new_fs - create new filesystem\n"
         "       -n - partition number\n"
         "       -p_* - parameters for file system tool\n";
    
    ExitProcess(0);
}

std::shared_ptr<Command> MakeImg::ParseNewImage(const vector<string>& inputTokens, uint64_t& inputPosition) {
    auto fileName      = inputTokens[inputPosition];
    auto sizeQualifier = inputTokens[inputPosition + 1];
    auto sizeValue     = inputTokens[inputPosition + 2];
    
    uint64_t sizeParsed = 0;
    try {
        sizeParsed = std::stoll(sizeValue);
    }
    catch (std::invalid_argument& e) {
        throw CommandParseException(inputPosition + 2, inputTokens[inputPosition + 2], "Invalid number");
    }
    
    uint64_t sizeInBytes = 0;
    if (sizeQualifier == "-sb")
        sizeInBytes = sizeParsed;
    else if (sizeQualifier == "-sk")
        sizeInBytes = sizeParsed * 1024UL;
    else if (sizeQualifier == "-sm")
        sizeInBytes = sizeParsed * 1024UL * 1024UL;
    else if (sizeQualifier == "-sg")
        sizeInBytes = sizeParsed * 1024UL * 1024UL * 1024UL;
    else if (sizeQualifier == "-ss")
        sizeInBytes = sizeParsed * 512;
    else
        throw CommandParseException(inputPosition + 1, inputTokens[inputPosition + 1], "Illegal size qualifier");
    
    inputPosition += 3;
    return shared_ptr<Command>(new NewImageCommand(fileName, sizeInBytes));
}

shared_ptr<Command> MakeImg::ParseLoadImage(const vector<string>& inputTokens, uint64_t& inputPosition) {
    return shared_ptr<Command>(new LoadImageCommand(inputTokens[inputPosition++]));
}

shared_ptr<Command> MakeImg::ParseNewTable(const vector<string>& inputTokens, uint64_t& inputPosition) {
    if (inputTokens[inputPosition] == "mbr") {
        if (inputTokens[inputPosition + 1][0] == '-') {
            if (inputTokens[inputPosition + 1] == "-b") {
                auto cmd = shared_ptr<Command>(new NewTableMbrCommand(inputTokens[inputPosition + 2]));
                inputPosition += 3;
                return cmd;
            }
            else {
                throw CommandParseException(
                    inputPosition,
                    inputTokens[inputPosition],
                    "Unknown option"
                );
            }
        }
        else {
            return shared_ptr<Command>(new NewTableMbrCommand());
        }
    }
    else if (inputTokens[inputPosition] == "gpt") {
        throw CommandParseException(
            inputPosition,
            inputTokens[inputPosition],
            "GPT is not supported yet"
        );
    }
    else if (inputTokens[inputPosition] == "loop") {
        inputPosition++;
        return shared_ptr<Command>(new NewTableLoopCommand());
    }
    else {
        throw CommandParseException(
            inputPosition,
            inputTokens[inputPosition],
            string("Unknown partition table type: '") + inputTokens[inputPosition] + "'"
        );
    }
}

shared_ptr<Command> MakeImg::ParseNewPart(const vector<string>& inputTokens, uint64_t& inputPosition) {
    auto number = inputTokens[inputPosition];
    
    uint64_t partNumber = 0;
    try {
        partNumber = std::stoll(number);
    }
    catch (std::invalid_argument& e) {
        throw CommandParseException(inputPosition + 2, inputTokens[inputPosition + 2], "Invalid number");
    }
    
    uint64_t    partBegin = -1;
    uint64_t    partEnd   = -1;
    set<string> flags;
    
    for (uint8_t i = 1;; ++i) {
        auto currentToken = inputPosition + i < inputTokens.size() ? inputTokens[inputPosition + i] : "";
        if (currentToken == "-b") {
            ++i;
            try {
                partBegin = std::stoll(inputTokens[inputPosition + i]);
            }
            catch (std::invalid_argument& e) {
                throw CommandParseException(inputPosition + i, inputTokens[inputPosition + i], "Invalid number");
            }
        }
        else if (currentToken == "-e") {
            ++i;
            try {
                partEnd = std::stoll(inputTokens[inputPosition + i]);
            }
            catch (std::invalid_argument& e) {
                throw CommandParseException(inputPosition + i, inputTokens[inputPosition + i], "Invalid number");
            }
        }
        else if (currentToken == "-f") {
            ++i;
            auto flagsStr    = inputTokens[inputPosition + i];
            auto flagsVector = StringUtil::Split(flagsStr, ',');
            flags = set<string>(flagsVector.begin(), flagsVector.end());
        }
        else if (currentToken[0] == '-') {
            throw CommandParseException(inputPosition + i, currentToken, "Unknown parameter");
        }
        else {
            // if current token is the next command
            inputPosition += i;
            return shared_ptr<Command>(
                new NewPartCommand(
                    partNumber,
                    partBegin,
                    partEnd,
                    flags
                )
            );
        }
    }
}

shared_ptr<Command> MakeImg::ParseNewFs(const vector<string>& inputTokens, uint64_t& inputPosition) {
    std::string                        fsType     = inputTokens[inputPosition];
    uint32_t                           partNumber = -1;
    std::map<std::string, std::string> fsParameters;
    
    for (uint8_t i = 1;; ++i) {
        std::string
            currentToken = inputPosition + i < inputTokens.size() ? inputTokens[inputPosition + i] : std::string("");
        if (currentToken == "-n") {
            ++i;
            try {
                partNumber = std::stoll(inputTokens[inputPosition + i]);
            }
            catch (std::invalid_argument& e) {
                throw CommandParseException(inputPosition + i, inputTokens[inputPosition + i], "Invalid number");
            }
        }
        else if (currentToken.rfind("-p_", 0) == 0) {
            fsParameters.insert(std::pair(currentToken, inputTokens[inputPosition + i + 1]));
            i++;
        }
        else if (currentToken[0] == '-') {
            throw CommandParseException(inputPosition + i, currentToken, "Unknown parameter");
        }
        else {
            // if current token is the next command
            inputPosition += i;
            if (partNumber == -1)
                throw CommandParseException(
                    inputPosition - 1,
                    inputTokens[inputPosition - 1],
                    "Partition number parameter not found"
                );
            return NewFsCommand::CreateFsCommandByType(fsType, partNumber, fsParameters);
        }
    }
}

shared_ptr<Command> MakeImg::ParseCopyFile(const vector<string>& inputTokens, uint64_t& inputPosition) {
    if ((inputTokens.size() - inputPosition) < 2) {
        throw CommandParseException(
            inputPosition - 1,
            inputTokens[inputPosition - 1],
            "There are not enough arguments; there are " +
            std::to_string(inputTokens.size() - inputPosition) +
            ", expected minimum 2"
        );
    }
    std::string       from = inputTokens[inputPosition++];
    std::string       to   = inputTokens[inputPosition++];
    std::vector<bool> attributes(3, false);
    
    while (inputTokens.size() != inputPosition) {
        std::string currentToken = inputTokens[inputPosition++];
        if (currentToken[0] != '-') {
            --inputPosition;
            break;
        }
        else {
            if (currentToken == "-r") {
                attributes[static_cast<int>(CopyFileAttributes::Recursive)] = true;
            }
            else if (currentToken == "-f") {
                attributes[static_cast<int>(CopyFileAttributes::Force)] = true;
            }
            else if (currentToken == "-pt") {
                attributes[static_cast<int>(CopyFileAttributes::PreserveTimestamps)] = true;
            }
            else {
                throw CommandParseException(
                    inputPosition - 1,
                    inputTokens[inputPosition - 1],
                    "Unknown Copy File Attribute"
                );
            }
        }
    }
    
    return shared_ptr<Command>(
        new CopyFileCommand(
            from,
            to,
            attributes
        )
    );
}

void MakeImg::Main(vector<string>& args) {
    if (args.empty())
        ExitProcess(1);
    if (args.at(0) == "help" || args.at(0) == "--help" || args.at(0) == "-h")
        ExitWithUsage();
    try {
        CommandSequence commandSequence;
        
        for (uint64_t i = 0; i < args.size();) {
            if (args[i] == "new_image") {
                commandSequence.Append(ParseNewImage(args, ++i));
            }
            else if (args[i] == "load_image") {
                commandSequence.Append(ParseLoadImage(args, ++i));
            }
            else if (args[i] == "new_table") {
                commandSequence.Append(ParseNewTable(args, ++i));
            }
            else if (args[i] == "new_part") {
                commandSequence.Append(ParseNewPart(args, ++i));
            }
            else if (args[i] == "new_fs") {
                commandSequence.Append(ParseNewFs(args, ++i));
            }
            else if (args[i] == "copy_file") {
                commandSequence.Append(ParseCopyFile(args, ++i));
            }
            else {
                throw CommandParseException(i, args[i], "Unknown command");
            }
        }
        
        Context context;
        commandSequence.Execute(context);
    }
    catch (std::exception& e) {
        ExitWithError(e.what(), -1);
    }
}




