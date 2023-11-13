#include "SymbolTable.h"

#include <iostream>
#include <iomanip>

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

void SymbolTable::addLiteral(string literal) {
    //Isolate the characters within the apostrophes
    size_t start = literal.find('\'') + 1;
    size_t end = literal.find('\'', start);
    string content = literal.substr(start, end - start);

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
    }

    //TODO: Add support for integer literals (is it required?)
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
    cout << "Symbol  Value   Flags:\n-----------------------" << endl;
    for(int i = 0; i < labels->size(); i++) {
        cout << labels->at(i);

        //Print spaces to format output correctly
        for(int j = 0; j < 8 - labels->at(i).length(); j++) cout << " ";

        cout << uppercase << hex << setw(6) << setfill('0') << symbolInfo->at(i).first << "  ";

        if(symbolInfo->at(i).second) cout << "R" << endl;
        else cout << "A" << endl;
    }
}