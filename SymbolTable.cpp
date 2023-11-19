#include "SymbolTable.h"

#include <iostream>
#include <iomanip>
#include <utility>
#include <sstream>

SymbolTable::SymbolTable() {
    labels = new vector<string>(0);
    //Symbol info format: <address, relative>
    symbolInfo = new vector<pair<unsigned int, bool>>(0);

    literals = new vector<string>(0);
    //Literal info format: <value, address, size>
    literalInfo = new vector<vector<unsigned int>>(0);
}
SymbolTable::~SymbolTable() {
    delete(labels);
    delete(symbolInfo);
    delete(literals);
    delete(literalInfo);
}

//Functions to set CSect name, starting address, and length (for printing)
void SymbolTable::setCSECT(string name, unsigned int address) {
    CSectName = std::move(name);
    startingAddress = address;
}
void SymbolTable::setLengthOfProgram(unsigned int length) {
    programLength = length;
}

//Helper function to print a specified number of spaces
void addSpaces(int number) {
    for(int i = 0; i < number; i++) {
        cout << " ";
    }
}

//Checks if the given string is a number or not
bool isStringNumber(const string& str) {
    istringstream iss(str);
    int num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

void SymbolTable::addSymbol(const std::string& symbolName, unsigned int address, bool relative) {
    labels->push_back(symbolName);
    symbolInfo->emplace_back(address, relative);
}
pair<int, bool> SymbolTable::getSymbolInfo(const std::string& symbolName) {
    //Find index of desired symbol in the list
    int index = -1;
    for(int i = 0; i < labels->size(); i++) {
        if(labels->at(i) == symbolName) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        //Symbol does not exist in the symbol table
        return pair<int, bool>{-1, false};
    } else {
        return symbolInfo->at(index);
    }
}
//Increments the addresses of every symbol and literal in the symbol table past 'address'
//Used when a format 3 instruction is converted to format 4 in pass two of the assembler
void SymbolTable::incrementSymbolAddresses(unsigned int address) {
    for(int i = 1; i < symbolInfo->size(); i++) {
        pair<unsigned int, bool>* symbol = &symbolInfo->at(i);
        if(symbol->first > address) {
            symbol->first += 1;
        }
    }

    for(auto& literal : *literalInfo) {
        if(literal.at(1) > address) {
            literal.at(1)++;
        }
    }
}

//Used to isolate the content of the literal (value between apostrophes)
string isolateLiteralContent(const string& literal) {
    size_t start = literal.find('\'') + 1;
    size_t end = literal.find('\'', start);
    return literal.substr(start, end - start);
}
//Converts a string such as X'F1' or C'EOF' to an unsigned int corresponding to its value
unsigned int SymbolTable::getValue(string operand) {
    //Check if the given literal is a decimal number
    if(isStringNumber(operand.substr(1))) return stoi(operand.substr(1));

    //Isolate the characters within the apostrophes
    string content = isolateLiteralContent(operand);

    if(operand[1] == 'C') {
        //Operand refers to a string
        //Iterate through the content and get the hex representation of the string
        unsigned int value = 0;
        for(char c : content) {
            value <<= 8;
            value += static_cast<int>(c);
        }
        return value;
    } else if(operand[1] == 'X') {
        //Operand is a hexadecimal number
        return static_cast<unsigned int>(stoi(content, nullptr, 16));
    }

    cout << "Error: could not process value of operand: " << operand << endl;
    exit(1);
}

void SymbolTable::addLiteral(string literal) {
    //Isolate the characters within the apostrophes
    string content = isolateLiteralContent(literal);
    unsigned int value = getValue(literal);
    literals->push_back(literal);

    if(literal[1] == 'C') {
        //Literal is a string
        literalInfo->push_back({value, 0, static_cast<unsigned int>(content.length())});
    } else if(literal[1] == 'X') {
        //Literal is a hexadecimal number
        literalInfo->push_back({value, 0, static_cast<unsigned int>((content.length() + 1) / 2)});
    } else {
        //Literal is a decimal number
        //Convert number to hex to find the number of bytes used
        stringstream stream;
        stream << hex << value;
        unsigned int length = (stream.str().length() + 1) / 2;
        literalInfo->push_back({value, 0, length});
    }
}
vector<unsigned int> SymbolTable::getLiteralInfo(const string& literalName) {
    //Find index of desired literal in literals list
    int index = -1;
    for(int i = 0; i < literals->size(); i++) {
        if(literals->at(i) == literalName) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        //Literal does not exist in the literal pool
        return vector<unsigned int>{};
    } else {
        return literalInfo->at(index);
    }
}
//Pools literals at the designated address
//Returns the new address after all literals have been pooled
//Alters the vector containing instructions to include the pooled literals
unsigned int SymbolTable::setLiteralsAtAddress(unsigned int address, vector<vector<string>>* instructions, unsigned int* addressCounter) {
    //Literals being pooled at the defined address
    //Iterate through the current literal pool, find ones not bound to an address, and pool them here
    unsigned int currentAddress = address;
    for(int i = 0; i < literals->size(); i++) {
        vector<unsigned int>* litInfo = &literalInfo->at(i);

        if(litInfo->at(1) == 0) {
            litInfo->at(1) = currentAddress;
            vector<string> instruction{to_string(currentAddress), "*", literals->at(i), ""};
            instructions->push_back(instruction);
            currentAddress += litInfo->at(2);
            *addressCounter += litInfo->at(2);
        }
    }

    return currentAddress;
}

void SymbolTable::printSymbols() {
    //Print symbols
    cout << "CSect   Symbol  Value   LENGTH  Flags:\n--------------------------------------" << endl;
    cout << CSectName;
    addSpaces(16 - CSectName.length());
    cout << uppercase << hex << setw(6) << setfill('0') << startingAddress << "  " << programLength << endl;

    for(int i = 0; i < labels->size(); i++) {
        addSpaces(8);
        cout << labels->at(i);

        //Print spaces to format output correctly
        addSpaces(8 - labels->at(i).length());

        cout << uppercase << hex << setw(6) << setfill('0') << symbolInfo->at(i).first;
        addSpaces(10);

        if(symbolInfo->at(i).second) cout << "R" << endl;
        else cout << "A" << endl;
    }

    //Print literals
    cout << endl << "Literal Table\nName  Operand   Address  Length:\n--------------------------------" << endl;
    for(int i = 0; i < literals->size(); i++) {
        string literal = isolateLiteralContent(literals->at(i));
        vector<unsigned int> info = literalInfo->at(i);

        cout << literal;
        addSpaces(6 - literal.length());
        cout << info[0];
        addSpaces(11 - to_string(info[0]).length());
        cout << info[1];
        addSpaces(10 - to_string(info[1]).length());
        cout << info[2] << endl;
    }
}

void SymbolTable::printLabels() {
    bool found = false;
    for(int i = 0; i < labels->size(); i++) {
        cout << labels->at(i) << endl;
        if(labels->at(i) == "TOTAL") found = true;
    }
    if(found) cout << "TOTAL found";
    else cout << "TOTAL not found";
}