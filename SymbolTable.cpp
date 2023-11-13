#include "SymbolTable.h"

#include <iostream>
#include <iomanip>
#include <utility>

SymbolTable::SymbolTable() {
    labels = new vector<string>(0);
    //Symbol info format: <address, relative>
    symbolInfo = new vector<pair<int, bool>>(0);

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
void SymbolTable::setCSECT(string name, int address) {
    CSectName = std::move(name);
    startingAddress = address;
}
void SymbolTable::setLengthOfProgram(int length) {
    programLength = length;
}

//Helper function to print a specified number of spaces
void addSpaces(int number) {
    for(int i = 0; i < number; i++) {
        cout << " ";
    }
}

void SymbolTable::addSymbol(const std::string& symbolName, int address, bool relative) {
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

//Used to isolate the content of the literal (value between apostrophes)
string isolateLiteralContent(const string& literal) {
    size_t start = literal.find('\'') + 1;
    size_t end = literal.find('\'', start);
    return literal.substr(start, end - start);
}

void SymbolTable::addLiteral(string literal) {
    //Isolate the characters within the apostrophes
    string content = isolateLiteralContent(literal);

    if(literal[1] == 'C') {
        //Literal is a string

        //Iterate through the content and get the hex representation of the string
        unsigned int value = 0;
        for(char c : content) {
            value <<= 8;
            value += static_cast<int>(c);
        }

        literals->push_back(literal);
        literalInfo->push_back({value, 0, static_cast<unsigned int>(content.length())});
    } else if(literal[1] == 'X') {
        //Literal is a hexadecimal number

        literals->push_back(literal);
        literalInfo->push_back({static_cast<unsigned int>(stoi(content, nullptr, 16)), 0, static_cast<unsigned int>(content.length())});
    }
}
vector<unsigned int> SymbolTable::getLiteralInfo(string literalName) {
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
void SymbolTable::setLiteralsAtAddress(unsigned int address) {
    //Literals being pooled at the defined address
    //Iterate through the current literal pool, find ones not bound to an address, and pool them here
    unsigned int currentAddress = address;
    for(auto & i : *literalInfo) {
        if(i[1] == 0) {
            i[1] = currentAddress;
            currentAddress += i[2];
        }
    }

    //TODO: Find some way to add literals to the list of instructions so they are printed in the listing file
    //TODO: Also find a way to increment the address counter in the assembler (pass in data object)
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