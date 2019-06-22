//
// Created by Stoorx on 19.06.2019.
//

#pragma once

#include <string>
#include <vector>
#include <ExitProcess.hpp>
#include <iostream>
#include <CommandParseException.hpp>
#include <Command.hpp>

using namespace std;

class MakeImg {
public:
    static void Main(vector<string>& args) {
        if (args.empty())
            ExitProcess(1);
        if (args.at(0) == "help")
            ExitWithUsage();
        
        for (int i = 0; i < args.size(); ++i) {
            if (args[i] == "new_image") {
                ParseNewImage(args, ++i);
            }
            
            if (args[i] == "load_image") {
                ParseLoadImage(args, ++i);
            }
        }
    }
    
    static Command* ParseNewImage(vector<string>& inputTokens, uint64_t inputPosition) {
        auto fileName = inputTokens[inputPosition];
        auto sizeQualifier = inputTokens[inputPosition + 1];
        auto sizeValue = inputTokens[inputPosition + 2];
    
        uint64_t sizeParsed = 0;
        try {
            sizeParsed = std::stoll(sizeValue);
        }catch (std::invalid_argument& e){
            throw CommandParseException(inputPosition, inputTokens[inputPosition], "Parse error");
        }
        
        uint64_t sizeInBytes = 0;
        if(sizeQualifier == "-sb")
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
            throw CommandParseException(inputPosition, inputTokens[inputPosition], "Illegal size qualifier");
    
        return new NewImageCommand(fileName, sizeInBytes);
        
    }
    
    static void ParseLoadImage(vector<string>& inputTokens, uint64_t inputPosition) {
    
    }
    
    static void ExitWithUsage() {
        cout <<
             "MakeImg - tool for creating disk images. Takes a scenario from command line and executes it step by step.\n"
             "Usage:\n"
             "   makeimg [new_image file_name -[sb|sk|sm|sg|ss] size]\n"
             "           [load_image img_file_name]\n"
             "           [new_table [mbr|gpt]]\n"
             "           [boot mbr_file]\n"
             "           [new_part partition_number -fs fs_type -b begin -e end [-p parameters]]\n"
             "Commands:\n"
             "   help - show this help\n"
             "   new_image - create new image file of given size\n"
             "       -sb - in bytes\n"
             "       -sk - in kilobytes\n"
             "       -sm - in megabytes\n"
             "       -sg - in gigabytes\n"
             "       -ss - in sectors\n"
             "   load_image - take an existing file to create image inside\n"
             "   new_table - create new partition table\n"
             "       mbr - MS-DOS style partition table\n"
             "       gpt - GPT style\n"
             "   boot - load bootsector code from given file (for mbr partition table only)\n"
             "   new_part - create new partition\n"
             "       -fs - set the type of filesystem (code or name)\n"
             "       -b - begin sector of the partition\n"
             "       -e - end sector of the partition\n"
             "       -fs* - parameters for FS formatting tool\n";
        
        ExitProcess(0);
    }
    
    static void ExitWithError(const string& message, int exitCode) {
        cout << message << endl;
        ExitProcess(exitCode);
    }
};



