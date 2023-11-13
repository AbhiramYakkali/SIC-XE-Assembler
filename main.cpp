#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iomanip>
#include <unordered_map>

#include "data.h"

#define NORMAL_EXIT 0
#define BAD_EXIT 1

//Helper function to print a specified number of spaces
void printSpaces(int number) {
    for(int i = 0; i < number; i++) {
        cout << " ";
    }
}

//Removes spaces at the end of a string
string removeSpaces(const string& str) {
    size_t lastNonSpaceCharacter = str.find_last_not_of(' ');
    string output;

    if(lastNonSpaceCharacter == 0) {
        output = "";
    } else {
        output = str.substr(0, lastNonSpaceCharacter + 1);
    }

    return output;
}

//Takes in a line of SIC/XE source code and returns its parts
//Returns a string array of format [label, opcode, operand]
vector<string> separateSourceLine(const string& line) {
    vector<string> output;

    output.push_back(removeSpaces(line.substr(0, 9)));
    output.push_back(removeSpaces(line.substr(9, 7)));
    output.push_back(removeSpaces(line.substr(17, 17)));

    return output;
}

//Creates op table
//Takes in the instruction name, returns a pair <opcode, format>, if format is 3/4, returns 3
unordered_map<string, pair<int, int>> createOPTable() {
    unordered_map<string, pair<int, int>> opTable;

    opTable.emplace("ADD", make_pair<int, int>(0x18, 3));
    opTable.emplace("ADDF", make_pair<int, int>(0x58, 3));
    opTable.emplace("ADDR", make_pair<int, int>(0x90, 2));
    opTable.emplace("AND", make_pair<int, int>(0x40, 3));
    opTable.emplace("CLEAR", make_pair<int, int>(0xB4, 2));
    opTable.emplace("COMP", make_pair<int, int>(0x28, 3));
    opTable.emplace("COMPF", make_pair<int, int>(0x88, 3));
    opTable.emplace("COMPR", make_pair<int, int>(0xA0, 2));
    opTable.emplace("DIV", make_pair<int, int>(0x24, 3));
    opTable.emplace("DIVF", make_pair<int, int>(0x64, 3));
    opTable.emplace("DIVR", make_pair<int, int>(0x9C, 3));
    opTable.emplace("FIX", make_pair<int, int>(0xC4, 1));
    opTable.emplace("FLOAT", make_pair<int, int>(0xC0, 1));
    opTable.emplace("HIO", make_pair<int, int>(0xF4, 1));
    opTable.emplace("J", make_pair<int, int>(0x3C, 3));
    opTable.emplace("JEQ", make_pair<int, int>(0x30, 3));
    opTable.emplace("JGT", make_pair<int, int>(0x34, 3));
    opTable.emplace("JLT", make_pair<int, int>(0x38, 3));
    opTable.emplace("JSUB", make_pair<int, int>(0x48, 3));
    opTable.emplace("LDA", make_pair<int, int>(0x00, 3));
    opTable.emplace("LDB", make_pair<int, int>(0x68, 3));
    opTable.emplace("LDCH", make_pair<int, int>(0x50, 3));
    opTable.emplace("LDF", make_pair<int, int>(0x70, 3));
    opTable.emplace("LDL", make_pair<int, int>(0x08, 3));
    opTable.emplace("LDS", make_pair<int, int>(0x6C, 3));
    opTable.emplace("LDT", make_pair<int, int>(0x74, 3));
    opTable.emplace("LDX", make_pair<int, int>(0x04, 3));
    opTable.emplace("LPS", make_pair<int, int>(0xD0, 3));
    opTable.emplace("MUL", make_pair<int, int>(0x20, 3));
    opTable.emplace("MULF", make_pair<int, int>(0x60, 3));
    opTable.emplace("MULR", make_pair<int, int>(0x98, 2));
    opTable.emplace("NORM", make_pair<int, int>(0xC8, 1));
    opTable.emplace("OR", make_pair<int, int>(0x44, 3));
    opTable.emplace("RD", make_pair<int, int>(0xD8, 3));
    opTable.emplace("RMO", make_pair<int, int>(0xAC, 2));
    opTable.emplace("RSUB", make_pair<int, int>(0x4C, 3));
    opTable.emplace("SHIFTL", make_pair<int, int>(0xA4, 2));
    opTable.emplace("SHIFTR", make_pair<int, int>(0xA8, 2));
    opTable.emplace("SIO", make_pair<int, int>(0xF0, 1));
    opTable.emplace("SSK", make_pair<int, int>(0xEC, 3));
    opTable.emplace("STA", make_pair<int, int>(0x0C, 3));
    opTable.emplace("STB", make_pair<int, int>(0x78, 3));
    opTable.emplace("STCH", make_pair<int, int>(0x54, 3));
    opTable.emplace("STF", make_pair<int, int>(0x80, 3));
    opTable.emplace("STI", make_pair<int, int>(0xD4, 3));
    opTable.emplace("STL", make_pair<int, int>(0x14, 3));
    opTable.emplace("STS", make_pair<int, int>(0x7C, 3));
    opTable.emplace("STSW", make_pair<int, int>(0xE8, 3));
    opTable.emplace("STT", make_pair<int, int>(0x84, 3));
    opTable.emplace("STX", make_pair<int, int>(0x10, 3));
    opTable.emplace("SUB", make_pair<int, int>(0x1C, 3));
    opTable.emplace("SUBF", make_pair<int, int>(0x5C, 3));
    opTable.emplace("SUBR", make_pair<int, int>(0x94, 2));
    opTable.emplace("SVC", make_pair<int, int>(0xB0, 2));
    opTable.emplace("TD", make_pair<int, int>(0xE0, 3));
    opTable.emplace("TIO", make_pair<int, int>(0xF8, 1));
    opTable.emplace("TIX", make_pair<int, int>(0x2C, 3));
    opTable.emplace("TIXR", make_pair<int, int>(0xB8, 2));
    opTable.emplace("WD", make_pair<int, int>(0xDC, 3));

    return opTable;
}

//Takes an operand (immediate operand, label, etc.) and converts it to target address
unsigned int convertOperandToTargetAddress(const string& operand, Data* data) {
    //In case of ',X' being present in operand, ignore it
    string shortenedOperand = operand.substr(0, operand.find(','));

    if(shortenedOperand[0] == '#') {
        //Immediate addressing
        //Check if symbol table contains the operand, if it doesn't, assume the operand is a number
        pair<int, bool> symbolInfo = data->symbolTable->getSymbolInfo(shortenedOperand.substr(1));

        if(symbolInfo.first == -1) {
            return stoi(shortenedOperand.substr(1));
        } else {
            return symbolInfo.first;
        }
    }
    if(shortenedOperand[0] == '@') {
        //Indirect addressing
    }
    if(shortenedOperand[0] == ' ') {
        //Simple addressing, get symbol address from symbol table and return
        return data->symbolTable->getSymbolInfo(shortenedOperand.substr(1)).first;
    }
    if(shortenedOperand[0] == '=') {
        return 0x2F07;
    }
}

//Process assembler directives, updating address counter and symbol table as necessary
void processAssemblerDirective(vector<string>* lineParts, Data* data) {
    string label = lineParts->at(0);
    string instruction = lineParts->at(1).substr(1);
    //TODO: Handle operands not being numbers
    string operand = lineParts->at(2);

    int address = data->currentAddress;
    SymbolTable* symbolTable = data->symbolTable;

    if(instruction == "START") {
        data->currentAddress = stoi(operand);
        data->symbolTable->setCSECT(label, stoi(operand));
    }
    if(instruction == "END") {
        //End of program, call command to pool literals at the current address
        data->symbolTable->setLiteralsAtAddress(data->currentAddress);
        data->symbolTable->setLengthOfProgram(data->currentAddress);
    }
    if(instruction == "RESW") {
        //Reserve word instruction, increment address counter by 3 times operand
        int numberOfWords = stoi(operand);
        symbolTable->addSymbol(label, address, true);
        data->currentAddress += numberOfWords * 3;
    }
    if(instruction == "RESB") {
        //Reserve byte instruction, increment address counter by operand
        int numberOfBytes = stoi(operand);
        symbolTable->addSymbol(label, address, true);
        data->currentAddress += numberOfBytes;
    }
    if(instruction == "BYTE") {
        //Byte instruction, increment address counter by one
        symbolTable->addSymbol(label, address, false);
        data->currentAddress++;
    }
    if(instruction == "WORD") {
        //Word instruction, increment address counter by three
        symbolTable->addSymbol(label, address, false);
        data->currentAddress += 3;
    }

    //TODO: Handle all other assembler directives
}

//Helper function to add bits to the end of an integer
unsigned int addBits(unsigned int input, const vector<int>& bits) {
    unsigned int output = input;

    for(int bit : bits) {
        output <<= 1;
        output += bit;
    }

    return output;
}
//Helper function to add a number to the end of an integer
unsigned int addNumber(unsigned int input, unsigned int numToAdd, int shiftAmount) {
    unsigned int output = input;
    output <<= shiftAmount;
    return output + numToAdd;
}

//Helper function to create a hash table mapping register to an int array corresponding to its number
unordered_map<char, vector<int>> initializeRegisterNumbers() {
    unordered_map<char, vector<int>> output;

    output['A'] = {0, 0, 0, 0};
    output['X'] = {0, 0, 0, 1};
    output['L'] = {0, 0, 1, 0};
    output['B'] = {0, 0, 1, 1};
    output['S'] = {0, 1, 0, 0};
    output['T'] = {0, 1, 0, 1};
    output['F'] = {0, 1, 1, 0};

    return output;
}

//Finds the addressing type of the given instruction
//Returns a vector containing the first two bits of nixbpe (n and i)
vector<int> findAddressingType(string operand) {
    char firstChar = operand[0];

    switch(firstChar) {
        case '@':
            return {1, 0};
        case '#':
            return {0, 1};
        default:
            return {1, 1};
    }
}

//Processes given instruction and returns object code
//Vector of size 4 stores instruction information: address, label, instruction, operand
unsigned int convertInstructionToObjectCode(vector<string> instruction, Data* data, unordered_map<string, pair<int, int>>* opTable) {
    pair<int, int> instructionInfo = opTable->at(instruction.at(2).substr(1));
    int format = instructionInfo.second;
    unsigned int objectCode;

    unsigned int targetAddress;

    //Don't calculate target address for format 3/4 instructions
    if(format == 3 || format == 4)  {
        if(instruction[3].empty()) return 0; //TODO: Handle format 3/4 instructions that don't take operands (ex: RSUB)
        else targetAddress = convertOperandToTargetAddress(instruction[3], data);
    }

    //Maps each register to an int array corresponding to its number (used in format 2)
    unordered_map<char, vector<int>> registerNumbers = initializeRegisterNumbers();

    //Check for '+' before instruction, switch to format 4 if found
    if(instruction[2][0] == '+') format = 4;

    if(format == 1) {
        //Format 1: object code = opcode
        return instructionInfo.first;
    } else if(format == 2) {
        //Format 2: object code = opcode (8 bits) + r1 (4 bits) + r2 (4 bits)
        objectCode = instructionInfo.first;
        objectCode = addBits(objectCode, registerNumbers[instruction.at(3)[1]]);
        objectCode = addBits(objectCode, registerNumbers[instruction.at(3)[3]]);
        return objectCode;
    } else if(format == 3) {
        //Format 3: opcode (6) + n i x b p e + disp (12)
        objectCode = instructionInfo.first;
        objectCode >>= 2;

        //Add first two bits (addressing type)
        objectCode = addBits(objectCode, findAddressingType(instruction[3]));

        //Add third bit (x bit)
        if(instruction[3].back() == 'X') objectCode = addBits(objectCode, {1});
        else objectCode = addBits(objectCode, {0});

        //Determine base/PC relative or direct addressing
        //Try PC relative addressing first, if address is too far away, try base relative, then direct
        int address = stoi(instruction[0]);
        int diff = targetAddress - address;
        objectCode = addBits(objectCode, {0, 1});

        //Add sixth bit (e bit); always 0 because this is format 3 instruction
        objectCode = addBits(objectCode, {0});

        //Add last 12 bits (displacement)
        //Mask last 12 bits of diff so that negative numbers are handled properly
        objectCode = addNumber(objectCode, diff & 0xFFF, 12);
        return objectCode;
    }
    if(format == 4) {
        //Format 4: opcode (6) + n i x b p e + address (20)
        objectCode = instructionInfo.first;
        objectCode >>= 2;

        //Add first two bits (addressing type)
        objectCode = addBits(objectCode, findAddressingType(instruction[3]));

        //Add third bit (x bit)
        if(instruction[3].back() == 'X') objectCode = addBits(objectCode, {1});
        else objectCode = addBits(objectCode, {0});

        //Add last 3 bits (b p e); always 0 0 1 because this is format 4 instruction
        objectCode = addBits(objectCode, {0, 0, 1});

        //Add last 20 bits (address)
        objectCode = addNumber(objectCode, targetAddress, 20);
        return objectCode;
    }
}

int main(int argc, char** argv) {
    if(argc == 1) {
        cout << "Invalid number of arguments; received 0, expected at least 1." << endl;
        exit(BAD_EXIT);
    }

    //REMEMBER TO USE .substr(1) ON INSTRUCTION WHEN SEARCHING THROUGH EITHER OF THESE LISTS
    //Initialize a vector containing all assembler directives
    set<string> assemblerDirectives{"START", "END", "RESB", "RESW", "BYTE", "WORD",
                                              "BASE", "*", "LTORG", "ORG", "EQU", "USE"};
    //Initialize a hashmap to store all instructions, opcodes, and formats
    //Call first for opcode, second for format
    unordered_map<string, pair<int, int>> opTable = createOPTable();

    //Initialize symbol table
    SymbolTable symbolTable;

    //Open source code file
    ifstream sourceFile(argv[1]);
    string line;

    //Initialize data object for ease of passing information to functions
    Data data;
    data.currentAddress = 0;
    data.baseRegister = -1;
    data.symbolTable = &symbolTable;

    //Vector to store data on instructions along with their calculated addresses
    //Allows pass two to skip reading the file and recalculating addresses, ignoring comments, etc.
    //Inner vector stores information for one instruction (size 4): address, label, instruction, operand
    vector<vector<string>> instructions;

    //Pass one of assembler
    //Process assembler directives, create symbol and literal table, process addresses of each instruction
    while(std::getline(sourceFile, line)) {
        //Skip comments
        if(line[0] == '.') continue;

        vector<string> lineParts = separateSourceLine(line);

        //Add current instruction to instructions vector (to be used in pass two)
        vector<string> instruction{to_string(data.currentAddress), lineParts.at(0), lineParts.at(1), lineParts.at(2)};
        instructions.push_back(instruction);

        if(assemblerDirectives.find(lineParts.at(1).substr(1)) != assemblerDirectives.end()) {
            //The current instruction is an assembler directive, must be processed
            processAssemblerDirective(&lineParts, &data);
        } else {
            //Current instruction is not an assembler directive
            //Check if current instruction is a symbol, add it to the symbol table if so
            if(!lineParts.at(0).empty()) {
                symbolTable.addSymbol(lineParts.at(0), data.currentAddress, true);
            }

            //Check if current instruction contains a literal, add it to the literal pool if so
            if(lineParts.at(2)[0] == '=') {
                symbolTable.addLiteral(lineParts.at(2));
            }

            //Increment address counter
            data.currentAddress += opTable[lineParts.at(1).substr(1)].second;

            if(lineParts.at(1)[0] == '+') data.currentAddress++;
        }
    }

    //Pass two of assembler
    //Convert instructions to object code, print to output file
    for(int i = 0; i < instructions.size(); i++) {
        vector<string> instruction = instructions.at(i);

        //Print instruction information (address, label, instruction, operand)
        cout << uppercase << hex << setw(4) << setfill('0') << stoi(instruction.at(0)) << "    ";
        cout << instruction.at(1);
        printSpaces(8 - instruction.at(1).length());
        cout << instruction.at(2);
        printSpaces(9 - instruction.at(2).length());
        cout << instruction.at(3);

        //Print object code of instruction
        if(assemblerDirectives.find(instruction.at(2).substr(1)) == assemblerDirectives.end()) {
            //Current instruction is not an assembler directive, convert instruction to object code and print
            instruction[0] = instructions.at(i + 1)[0];
            unsigned int objectCode = convertInstructionToObjectCode(instruction, &data, &opTable);

            //Number of characters displayed in object code depends on format
            int format = opTable.at(instruction.at(2).substr(1)).second;
            if(instruction.at(2)[0] == '+') format++;

            printSpaces(26 - instruction.at(3).length());
            cout << uppercase << hex << setw(2 * format) << setfill('0') << objectCode << endl;
        } else {
            cout << endl;
        }
    }

    cout << endl << endl;
    symbolTable.printSymbols();

    return NORMAL_EXIT;
}