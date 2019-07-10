//
// Created by Stoorx on 19.06.2019.
//

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
         "            [new_table [mbr|gpt] -b mbr_file]\n"
         "            [new_part partition_number -b begin -e end -f flags]"
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
         "       -f - partition flags"
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
    
    uint64_t numberParsed = 0;
    try {
        numberParsed = std::stoll(number);
    }
    catch (std::invalid_argument& e) {
        throw CommandParseException(inputPosition + 2, inputTokens[inputPosition + 2], "Invalid number");
    }
    
    // TODO: logic
    return nullptr;
}

void MakeImg::Main(vector<string>& args) {
    if (args.empty())
        ExitProcess(1);
    if (args.at(0) == "help")
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


