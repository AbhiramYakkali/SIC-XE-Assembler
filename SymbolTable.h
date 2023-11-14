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

    void addSymbol(const string& symbolName, int address, bool relative);
    pair<int, bool> getSymbolInfo(const string& symbolName);

    void addLiteral(string literal);
    vector<unsigned int> getLiteralInfo(string literalName);
    unsigned int setLiteralsAtAddress(unsigned int address, vector<vector<string>>* instructions);

    void setCSECT(string name, int address);
    void setLengthOfProgram(int length);

    void printSymbols();
};