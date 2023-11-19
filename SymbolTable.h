#include <string>
#include <vector>

using namespace std;

class SymbolTable {
private:
    vector<string> *labels;
    vector<pair<unsigned int, bool>> *symbolInfo;

    vector<string> *literals;
    vector<vector<unsigned int>> *literalInfo;

    string CSectName;
    unsigned int startingAddress{}, programLength{};

public:
    SymbolTable();
    ~SymbolTable();

    static unsigned int getValue(string operand);

    void addSymbol(const string& symbolName, unsigned int address, bool relative);
    pair<int, bool> getSymbolInfo(const string& symbolName);
    void incrementSymbolAddresses(unsigned int address);

    void addLiteral(string literal);
    vector<unsigned int> getLiteralInfo(const string& literalName);
    unsigned int setLiteralsAtAddress(unsigned int address, vector<vector<string>>* instructions, unsigned int* addressCounter);

    void setCSECT(string name, unsigned int address);
    void setLengthOfProgram(unsigned int length);

    void printSymbols();
    void printLabels();
};