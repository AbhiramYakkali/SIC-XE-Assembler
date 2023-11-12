#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iomanip>

#include "data.h"

#define NORMAL_EXIT 0
#define BAD_EXIT 1

//Helper function to print a specified number of spaces
void printSpaces(int number) {
    for(int i = 0; i < number; i++) {
        cout << " ";
    }
}

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

    //if(output[0] == ' ') return output.substr(1);
    //else return output;
    return output;
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
void processAssemblerDirective(std::vector<std::string>* lineParts, Data* data) {
    std::string label = lineParts->at(0);
    std::string instruction = lineParts->at(1);
    std::string operand = lineParts->at(2);

    int address = data->currentAddress;
    SymbolTable* symbolTable = data->symbolTable;

    if(instruction == "RESW") {
        //Reserve word instruction, increment address counter by 3 times operand
        int numberOfWords = std::stoi(operand);
        symbolTable->addSymbol(label, address, true);
        data->currentAddress += numberOfWords * 3;
        return;
    }
    if(instruction == "RESB") {
        //Reserve byte instruction, increment address counter by operand
        int numberOfBytes = std::stoi(operand);
        symbolTable->addSymbol(label, address, true);
        data->currentAddress += numberOfBytes;
    }
    if(instruction == "BYTE") {
        //Byte instruction, increment address counter by one
        symbolTable->addSymbol(label, address, false);
        data->currentAddress++;
    }
    if(instruction == "WORD") {
        //Word instruction, increment address counter by three
        symbolTable->addSymbol(label, address, false);
        data->currentAddress += 3;
    }

    //TODO: Handle all other assembler directives
}

//Processes given instruction and returns object code
//Vector of size 4 stores instruction information: address, label, instruction, operand
unsigned int convertInstructionToObjectCode(std::vector<std::string> instruction, Data* data) {
    return 0;
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

    //Initialize data object for ease of passing information to functions
    Data data;
    data.currentAddress = 0;
    data.baseRegister = 0;
    data.symbolTable = &symbolTable;

    //Vector to store data on instructions along with their calculated addresses
    //Allows pass two to skip reading the file and recalculating addresses, ignoring comments, etc.
    //Inner vector stores information for one instruction (size 4): address, label, instruction, operand
    std::vector<std::vector<std::string>> instructions;

    //Pass one of assembler
    //Process assembler directives, create symbol and literal table, process addresses of each instruction
    while(std::getline(sourceFile, line)) {
        //Skip comments
        if(line[0] == '.') continue;

        std::vector<std::string> lineParts = separateSourceLine(line);

        //Add current instruction to instructions vector (to be used in pass two)
        std::vector<std::string> instruction{std::to_string(data.currentAddress), lineParts.at(0), lineParts.at(1), lineParts.at(2)};
        instructions.push_back(instruction);

        if(assemblerDirectives.find(lineParts.at(1)) != assemblerDirectives.end()) {
            //The current instruction is an assembler directive
            processAssemblerDirective(&lineParts, &data);
        } else {
            //Current instruction is not an assembler directive
            //Check if current instruction is a symbol, add it to the symbol table if so
            if(!lineParts.at(0).empty()) {
                symbolTable.addSymbol(lineParts.at(0), data.currentAddress, true);
            }

            //TODO: Alter this code to work with format 1/2 instructions
            //Increment address counter
            data.currentAddress += 3;

            if(lineParts.at(1)[0] == '+') data.currentAddress++;
        }
    }

    //Pass two of assembler
    //Convert instructions to object code, print to output file
    for(const auto& instruction : instructions) {
        unsigned int objectCode = convertInstructionToObjectCode(instruction, &data);

        //Print out each instruction in the proper output format
        cout << uppercase << hex << setw(4) << setfill('0') << stoi(instruction.at(0)) << " ";
        cout << instruction.at(1);
        printSpaces(8 - instruction.at(1).length());
        cout << instruction.at(2);
        printSpaces(8 - instruction.at(2).length());
        cout << instruction.at(3);
        printSpaces(12 - instruction.at(3).length());
        cout << uppercase << hex << setw(6) << setfill('0') << objectCode << endl;
    }

    //symbolTable.printSymbols();

    return NORMAL_EXIT;
}