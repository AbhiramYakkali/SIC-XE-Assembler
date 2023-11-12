#include <string>
#include <vector>

using namespace std;

class SymbolTable {
private:
    vector<string> *labels;
    vector<pair<int, bool>> *symbolInfo;

public:
    SymbolTable();
    ~SymbolTable();

    void addSymbol(const string& symbolName, int address, bool relative);
    pair<int, bool> getSymbolInfo(const string& symbolName);
    void printSymbols();
};