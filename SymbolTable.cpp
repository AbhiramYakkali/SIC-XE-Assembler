#include "SymbolTable.h"

#include <iostream>
#include <iomanip>

SymbolTable::SymbolTable() {
    labels = new vector<string>(0);
    symbolInfo = new vector<pair<int, bool>>(0);
}
SymbolTable::~SymbolTable() {
    delete(labels);
    delete(symbolInfo);
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

void SymbolTable::printSymbols() {
    cout << "Symbol  Value   Flags:\n-----------------------" << endl;
    for(int i = 0; i < labels->size(); i++) {
        cout << labels->at(i);

        //Print spaces to format output correctly
        for(int j = 0; j < 8 - labels->at(i).length(); j++) cout << " ";

        cout << std::uppercase << std::hex << std::setw(6) << std::setfill('0') << symbolInfo->at(i).first << "  ";

        if(symbolInfo->at(i).second) cout << "R" << endl;
        else cout << "A" << endl;
    }
}