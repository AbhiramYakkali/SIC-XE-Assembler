#include "SymbolTable.h"

typedef struct {
    unsigned int currentAddress;
    unsigned int baseRegister;
    bool baseRegisterValid;
    //Tracks how much should be added to the address of instructions
    //Incremented when a format 3 instruction is switched to format 4 in pass two
    int additionalAddressCounter;

    SymbolTable* symbolTable;

    vector<vector<string>>* convertedInstructions;
    vector<bool>* mustRecalculateObjectCode;
    unordered_map<string, pair<int, int>>* opTable;
} Data;