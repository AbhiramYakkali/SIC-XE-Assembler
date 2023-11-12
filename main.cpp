#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "SymbolTable.h"

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

//Process assembler directives, updating address counter and symbol table as necessary
void processAssemblerDirective(int* address, std::vector<std::string>* lineParts, SymbolTable* symbolTable) {
    std::string label = lineParts->at(0);
    std::string instruction = lineParts->at(1);
    std::string operand = lineParts->at(2);

    if(instruction == "RESW") {
        //Reserve word instruction, increment address counter by 3 times operand
        int numberOfWords = std::stoi(operand);
        symbolTable->addSymbol(label, *address, true);
        *address += numberOfWords * 3;
        return;
    }
    if(instruction == "RESB") {
        //Reserve byte instruction, increment address counter by operand
        int numberOfBytes = std::stoi(operand);
        symbolTable->addSymbol(label, *address, true);
        *address += numberOfBytes;
    }
    if(instruction == "BYTE") {
        //Byte instruction, increment address counter by one
        symbolTable->addSymbol(label, *address, false);
        *address += 1;
    }
    if(instruction == "WORD") {
        //Word instruction, increment address counter by three
        symbolTable->addSymbol(label, *address, false);
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
    //Initialize symbol table
    SymbolTable symbolTable;

    std::ifstream sourceFile(argv[1]);
    std::string line;
    int currentAddress = 0;

    while(std::getline(sourceFile, line)) {
        //Skip comments
        if(line[0] == '.') continue;

        //std::cout << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << currentAddress << " ";
        //std::cout << line << std::endl;

        std::vector<std::string> lineParts = separateSourceLine(line);

        if(assemblerDirectives.find(lineParts.at(1)) != assemblerDirectives.end()) {
            //The current instruction is an assembler directive
            processAssemblerDirective(&currentAddress, &lineParts, &symbolTable);
        } else {
            //Current instruction is not an assembler directive
            //Check if current instruction is a symbol, add it to the symbol table if so
            if(!lineParts.at(0).empty()) {
                symbolTable.addSymbol(lineParts.at(0), currentAddress, true);
            }

            //TODO: Alter this code to work with format 1/2 instructions
            //Increment address counter
            currentAddress += 3;

            if(lineParts.at(1)[0] == '+') currentAddress++;
        }
    }

    symbolTable.printSymbols();

    return NORMAL_EXIT;
}