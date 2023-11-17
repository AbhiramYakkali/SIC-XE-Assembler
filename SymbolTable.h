#include <string>
#include <vector>

using namespace std;

class SymbolTable {
private:
    vector<string> *labels;
    vector<pair<int, bool>> *symbolInfo;

    vector<string> *literals;
    vector<vector<unsigned int>> *literalInfo;

    string CSectName;
    int startingAddress, programLength;

public:
    SymbolTable();
    ~SymbolTable();

    static unsigned int getValue(string operand);

    void addSymbol(const string& symbolName, int address, bool relative);
    pair<int, bool> getSymbolInfo(const string& symbolName);
    void incrementSymbolAddresses(unsigned int address);

    void addLiteral(string literal);
    vector<unsigned int> getLiteralInfo(const string& literalName);
    unsigned int setLiteralsAtAddress(unsigned int address, vector<vector<string>>* instructions, int* addressCounter);

    void setCSECT(string name, int address);
    void setLengthOfProgram(int length);

    void printSymbols();
};