#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstring>
#include <set>

#define NORMAL_EXIT 0
#define BAD_EXIT 1

//Removes spaces at the end of a string
//Removes space at the beginning of a string if applicable
std::string removeSpaces(const std::string& str) {
    size_t lastNonSpaceCharacter = str.find_last_not_of(' ');
    std::string output;

    if(lastNonSpaceCharacter == 0) {
        output = "";
    } else {
        output = str.substr(0, lastNonSpaceCharacter + 1);
    }

    if(output[0] == ' ') return output.substr(1);
    else return output;
}

//Takes in a line of SIC/XE source code and returns its parts
//Returns a string array of format [label, opcode, operand]
std::vector<std::string> separateSourceLine(const std::string& line) {
    std::vector<std::string> output;

    output.push_back(removeSpaces(line.substr(0, 8)));
    output.push_back(removeSpaces(line.substr(8, 6)));
    output.push_back(removeSpaces(line.substr(16, 17)));

    return output;
}

//Process assembler directives, updating the address counter as necessary
//Should update symbol table (once implemented)
void processAssemblerDirective(int* address, const std::string& instruction, const std::string& operand) {
    if(instruction == "RESW") {
        //Reserve word instruction, increment address counter by 3 times operand
        int numberOfWords = std::stoi(operand);
        *address += numberOfWords * 3;
        return;
    }
    if(instruction == "RESB") {
        //Reserve byte instruction, increment address counter by operand
        int numberOfBytes = std::stoi(operand);
        *address += numberOfBytes;
    }
    if(instruction == "BYTE") {
        //Byte instruction, increment address counter by one
        *address += 1;
    }
    if(instruction == "WORD") {
        //Word instruction, increment address counter by three
        *address += 3;
    }

    //TODO: Handle all other assembler directives
}

int main(int argc, char** argv) {
    if(argc == 1) {
        std::cout << "Invalid number of arguments; received 0, expected at least 1." << std::endl;
        exit(BAD_EXIT);
    }

    //Initialize a vector containing all assembler instructions
    std::set<std::string> assemblerDirectives{"START", "END", "RESB", "RESW", "BYTE", "WORD",
                                              "BASE", "END", "*", "LTORG", "ORG", "EQU", "USE"};

    std::ifstream sourceFile(argv[1]);
    std::string line;
    int currentAddress = 0;

    while(std::getline(sourceFile, line)) {
        //Skip comments
        if(line[0] == '.') continue;

        std::cout << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << currentAddress << " ";
        std::cout << line << std::endl;

        std::vector<std::string> lineParts = separateSourceLine(line);

        if(assemblerDirectives.find(lineParts.at(1)) != assemblerDirectives.end()) {
            //The current instruction is an assembler directive
            processAssemblerDirective(&currentAddress, lineParts.at(1), lineParts.at(2));
        } else {
            //Current instruction is not an assembler directive, so no additional work needs to be done
            //TODO: Alter this code to work with format 1/2 instructions
            currentAddress += 3;

            if(lineParts.at(1)[0] == '+') currentAddress++;
        }
    }

    return NORMAL_EXIT;
}