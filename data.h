#include "SymbolTable.h"

typedef struct {
    int currentAddress;
    int baseRegister;

    SymbolTable* symbolTable;
} Data;