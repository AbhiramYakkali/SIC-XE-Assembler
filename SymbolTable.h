#include <string>
#include <vector>

using namespace std;

class SymbolTable {
private:
    vector<string> *labels;
    vector<pair<int, bool>> *symbolInfo;

    vector<string> *literals;
    vector<vector<unsigned int>> *literalInfo;

public:
    SymbolTable();
    ~SymbolTable();

    void addSymbol(const string& symbolName, int address, bool relative);
    pair<int, bool> getSymbolInfo(const string& symbolName);

    void addLiteral(string literal);
    vector<unsigned int> getLiteralInfo(string literalName);
    void setLiteralsAtAddress(unsigned int address);

    void printSymbols();
};