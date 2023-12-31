#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <iomanip>
#include <unordered_map>
#include <sstream>

#include "data.h"

#define NORMAL_EXIT 0
#define BAD_EXIT 1
#define INTERNAL_ERROR 2

unsigned int convertInstructionToObjectCode(vector<string>* instruction, Data* data, int index);

//Helper function to print a specified number of spaces
void printSpacesToFile(int number, ofstream* file) {
    for(int i = 0; i < number; i++) {
        *file << " ";
    }
}

//Helper function to convert a number to a string in hex format
string convertNumberToHex(unsigned int number, int requiredLength) {
    stringstream stream;
    stream << uppercase << hex << setw(requiredLength) << setfill('0') << number;
    return stream.str();
}

//Removes spaces at the end of a string
string removeSpaces(const string& str) {
    size_t lastNonSpaceCharacter = str.substr(1).find(' ');
    string output;

    output = str[0] + str.substr(1, lastNonSpaceCharacter);

    return output;
}

//Takes in a line of SIC/XE source code and returns its parts
//Returns a string array of format [label, opcode, operand]
vector<string> separateSourceLine(const string& line) {
    vector<string> output;

    output.push_back(removeSpaces(line.substr(0, 9)));
    output.push_back(removeSpaces(line.substr(9, 7)));
    if(line.length() < 17) output.emplace_back("");
    else output.push_back(removeSpaces(line.substr(17, 17)));

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

//Checks if the given string is a number or not
bool isStringANumber(const string& str) {
    istringstream iss(str);
    int num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

//Returns a pair: <evaluated value of expression, if the result is relative>
pair<unsigned int, bool> evaluateExpression(const string& operand1, const string& operand2, char operation, Data* data) {
    int numOfRelative = 0;
    unsigned int convertedOperand1, convertedOperand2;

    if(isStringANumber(operand1)) {
        convertedOperand1 = stoi(operand1);
    } else {
        pair<unsigned int, bool> operandInfo = data->symbolTable->getSymbolInfo(operand1);
        convertedOperand1 = operandInfo.first;
        if(operandInfo.second) numOfRelative++;
    }
    if(isStringANumber(operand2)) {
        convertedOperand2 = stoi(operand2);
    } else {
        pair<unsigned int, bool> operandInfo = data->symbolTable->getSymbolInfo(operand2);
        convertedOperand2 = operandInfo.first;
        if(operandInfo.second) numOfRelative++;
    }

    //Perform specified operations, carry out necessary checks (based on number of relative terms)
    if(operation == '+') {
        if(numOfRelative == 2) {
            //Adding two relative symbols is considered an error
            cout << "Error: attempted to add two relative symbols: " << operand1 << ", " << operand2 << endl;
            exit(BAD_EXIT);
        }
        return make_pair(convertedOperand1 + convertedOperand2, numOfRelative == 1);
    }
    if(operation == '-') {
        return make_pair(convertedOperand1 - convertedOperand2, numOfRelative == 1);
    }
    if(operation == '*') {
        if(numOfRelative == 1) {
            //Multiplying a relative symbol by an absolute is considered an error
            cout << "Error: attempted to multiply a relative symbol by an absolute: "
                 << operand1 << ", " << operand2 << endl;
        }
        return make_pair(convertedOperand1 * convertedOperand2, numOfRelative == 1);
    }
    if(operation == '/') {
        if(numOfRelative == 1) {
            //Dividing a relative symbol by an absolute is considered an error
            cout << "Error: attempted to perform division between relative and absolute symbols: " <<
                 operand1 << ", " << operand2 << endl;
        }
        return make_pair(convertedOperand1 / convertedOperand2, numOfRelative == 1);
    }

    cout << "Internal error: received invalid operation: " << operation << endl;
    exit(INTERNAL_ERROR);
}

//Takes an operand (immediate operand, label, etc.) and converts it to target address
//Returns a pair: <target address, if the result is relative> (sometimes result is not relative, ex: if it is a number)
pair<unsigned int, bool> convertOperandToTargetAddress(const string& operand, Data* data) {
    //In case of ',X' being present in operand, ignore it
    string shortenedOperand = operand.substr(0, operand.find(','));
    //Isolate first character because it usually indicates what type of operand this is
    char firstChar = shortenedOperand[0];

    //Check if operand is a number
    if(isStringANumber(shortenedOperand)) return make_pair(stoi(shortenedOperand), false);

    if(shortenedOperand == "*") return make_pair(data->currentAddress, true);
    if(firstChar == '=') {
        //Operand is a literal, get address from symbol table
        return make_pair(data->symbolTable->getLiteralInfo(shortenedOperand).at(1), true);
    }
    //Check if operand is an expression
    string operations = "+-*/";
    for(char op : operations) {
        size_t index = shortenedOperand.find(op);
        if(index != string::npos) {
            //The string contains the operation being checked
            string operand1 = shortenedOperand.substr(1, index - 1);
            string operand2 = shortenedOperand.substr(index + 1);

            return evaluateExpression(operand1, operand2, op, data);
        }
    }

    if((shortenedOperand[1] == 'C' || shortenedOperand[1] == 'X') && shortenedOperand[2] == '\'') {
        //This is an operand of format X'F1' or C'EOF'
        //Use SymbolTable function to find its value
        return make_pair(SymbolTable::getValue(shortenedOperand), false);
    }
    if(firstChar == '#') {
        //Immediate addressing
        //Check if symbol table contains the operand, if it doesn't, assume the operand is a number
        pair<int, bool> symbolInfo = data->symbolTable->getSymbolInfo(shortenedOperand.substr(1));

        if(symbolInfo.first == -1) {
            return make_pair(stoi(shortenedOperand.substr(1)), false);
        } else {
            return make_pair(symbolInfo.first, true);
        }
    }
    if(firstChar == ' ' || firstChar == '@') {
        //Simple/Indirect addressing, get symbol address from symbol table and return
        return make_pair(data->symbolTable->getSymbolInfo(shortenedOperand.substr(1)).first, true);
    }

    //Error: given operand does not match any of the recognized patterns
    cout << "Error: could not parse the given operand: " << operand << endl;
    exit(BAD_EXIT);
}

//Process assembler directives, updating address counter and symbol table as necessary
void processAssemblerDirective(vector<string>* lineParts, Data* data, vector<vector<string>>* instructions) {
    string label = lineParts->at(0);
    string instruction = lineParts->at(1).substr(1);
    string operand = lineParts->at(2);
    unsigned int targetAddress;
    bool targetAddressRelative;
    if(!operand.empty()) {
        pair<unsigned int, bool> convertedOperand = convertOperandToTargetAddress(operand, data);
        targetAddress = convertedOperand.first;
        targetAddressRelative = convertedOperand.second;
    }

    unsigned int address = data->currentAddress;
    SymbolTable* symbolTable = data->symbolTable;

    if(instruction == "START") {
        data->currentAddress = stoi(operand, nullptr, 16);
        data->symbolTable->setCSECT(label, stoi(operand));
    }
    if(instruction == "END") {
        //End of program, call command to pool literals at the current address
        data->currentAddress = data->symbolTable->setLiteralsAtAddress(data->currentAddress, instructions, &data->currentAddress);
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
    if(instruction == "LTORG") {
        //LTORG instruction, pool all unpooled literals at the current address
        symbolTable->setLiteralsAtAddress(data->currentAddress, instructions, &data->currentAddress);
    }
    if(instruction == "EQU") {
        //EQU instruction, symbol value is the calculated operand
        symbolTable->addSymbol(label, address, targetAddressRelative);
        string operandInHex = convertNumberToHex(targetAddress, 0);
        data->currentAddress += (operandInHex.length() + 1) / 2;
    }
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

//The following three functions check if each type of addressing works: PC relative, base relative, direct
//Returns a pair <valid, address>, check valid boolean first to see if the addressing mode works
pair<bool, unsigned int> testPCRelativeAddressing(unsigned int targetAddress, int address, unsigned int objectCode) {
    int diff = targetAddress - address;
    if(diff >= -2048 && diff <= 2047) {
        //Meets conditions for PC relative addressing, use PC relative addressing
        objectCode = addBits(objectCode, {0, 1, 0});
        //Add last 12 bits (displacement)
        //Mask last 12 bits of diff so that negative numbers are handled properly
        objectCode = addNumber(objectCode, diff & 0xFFF, 12);
        return make_pair(true, objectCode);
    } else {
        return make_pair(false, 0);
    }
}
pair<bool, unsigned int> testBaseRelativeAddressing(unsigned int targetAddress, Data* data, unsigned int objectCode) {
    unsigned int base = data->baseRegister;

    if(!data->baseRegisterValid || targetAddress < base) return make_pair(false, 0);

    if(targetAddress - base <= 4095) {
        //Meets conditions for base relative addressing, use base relative addressing
        objectCode = addBits(objectCode, {1, 0, 0});
        //Add last 12 bits (displacement), masking not necessary because diff must be positive
        objectCode = addNumber(objectCode, targetAddress - base, 12);
        return make_pair(true, objectCode);
    } else {
        return make_pair(false, 0);
    }
}
pair<bool, unsigned int> testDirectAddressing(unsigned int targetAddress, unsigned int objectCode) {
    if(targetAddress <= 4095) {
        //Meets conditions for direct addressing, use direct addressing
        objectCode = addBits(objectCode, {0, 0, 0});
        //Add last 12 bits (address), masking not necessary because address must be positive
        objectCode = addNumber(objectCode, targetAddress, 12);
        return make_pair(true, objectCode);
    } else {
        return make_pair(false, 0);
    }
}

//Recalculates object codes for all prior instructions that refer to a symbol
//Used when a format 3 instruction is converted to format 4 in pass two
void recalculateInstructionObjectCodes(unsigned int addressOfLastInstruction, Data* data) {
    vector<vector<string>>* instructions = data->convertedInstructions;
    vector<bool>* targetAddresses = data->mustRecalculateObjectCode;

    for(int i = 0; i < instructions->size() - 1; i++) {
        vector<string>* instruction = &instructions->at(i);

        //BASE and NOBASE instructions should be processed here to make sure base register is kept up to date
        if(instruction->at(2) == " BASE") {
            data->baseRegister = convertOperandToTargetAddress(instruction->at(3), data).first;
            data->baseRegisterValid = true;
            continue;
        }
        if(instruction->at(2) == " NOBASE") {
            data->baseRegisterValid = false;
            continue;
        }

        if(targetAddresses->at(i)) {
            string instructionAddress = instruction->at(0);

            //Convert address of instruction to decimal because 'convertInstructionToObjectCode' takes decimal addresses
            instruction->at(0) = to_string(stoi(instruction->at(0), nullptr, 16));

            unsigned int newObjectCode = convertInstructionToObjectCode(instruction, data, i);
            instruction->at(4) = convertNumberToHex(newObjectCode, instruction->at(4).length());
            instruction->at(0) = instructionAddress;
        }
    }
}

void updateTargetAddressVector(int index, bool relative, Data* data) {
    if(index == -1) {
        data->mustRecalculateObjectCode->push_back(relative);
    } else {
        data->mustRecalculateObjectCode->at(index) = relative;
    }
}

//Processes given instruction and returns object code
//Vector of size 4 stores instruction information: address, label, instruction, operand
unsigned int convertInstructionToObjectCode(vector<string>* instruction, Data* data, int index) {
    unordered_map<string, pair<int, int>>* opTable = data->opTable;
    vector<bool>* mustRecalculateObjectCode = data->mustRecalculateObjectCode;

    //Check if the instruction exists in the optable (checking if it is a valid instruction)
    if(opTable->count(instruction->at(2).substr(1)) == 0) {
        cout << data->convertedInstructions->size() << endl;
        cout << "Error: instruction not found in op table: " << instruction->at(2).substr(1) << endl;
        exit(BAD_EXIT);
    }

    pair<int, int> instructionInfo = opTable->at(instruction->at(2).substr(1));
    int format = instructionInfo.second;
    unsigned int objectCode;

    unsigned int targetAddress;

    //Don't calculate target address for format 3/4 instructions
    if(format == 3 || format == 4)  {
        //RSUB is an exception, it is a format 3 instruction but doesn't take an operand
        if(instruction->at(2).find("RSUB") != string::npos) {
            mustRecalculateObjectCode->push_back(false);
            //5177344 = 0x4F0000
            return 5177344;
        }
        else targetAddress = convertOperandToTargetAddress(instruction->at(3), data).first;
    }

    //Check for '+' before instruction, switch to format 4 if found
    if(instruction->at(2)[0] == '+') format = 4;

    if(format == 1) {
        //Format 1: object code = opcode
        mustRecalculateObjectCode->push_back(false);
        return instructionInfo.first;
    } else if(format == 2) {
        //Format 2: object code = opcode (8 bits) + r1 (4 bits) + r2 (4 bits)
        //Maps each register to an int array corresponding to its number
        unordered_map<char, vector<int>> registerNumbers = initializeRegisterNumbers();

        objectCode = instructionInfo.first;
        objectCode = addBits(objectCode, registerNumbers[instruction->at(3)[1]]);
        objectCode = addBits(objectCode, registerNumbers[instruction->at(3)[3]]);
        mustRecalculateObjectCode->push_back(false);
        return objectCode;
    } else if(format == 3) {
        //Format 3: opcode (6) + n i x b p e + disp (12)
        objectCode = instructionInfo.first;
        objectCode >>= 2;

        //Add first two bits (addressing type)
        objectCode = addBits(objectCode, findAddressingType(instruction->at(3)));

        //Add third bit (x bit)
        if(instruction->at(3).back() == 'X') objectCode = addBits(objectCode, {1});
        else objectCode = addBits(objectCode, {0});

        //Determine addressing mode
        int address = stoi(instruction->at(0));

        if(instruction->at(3)[0] == '#') {
            //Using immediate addressing
            //Addressing mode order: direct, PC relative, base relative

            //Try direct addressing
            pair<bool, unsigned int> result = testDirectAddressing(targetAddress, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, false, data);
                return result.second;
            }

            //Try PC relative addressing
            result = testPCRelativeAddressing(targetAddress, address, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, true, data);
                return result.second;
            }

            //Try base relative addressing
            result = testBaseRelativeAddressing(targetAddress, data, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, true, data);
                return result.second;
            }
        } else {
            //Simple/indirect addressing
            //Addressing mode order: PC relative, base relative, direct

            //Try PC relative addressing
            pair<bool, unsigned int> result = testPCRelativeAddressing(targetAddress, address, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, true, data);
                return result.second;
            }

            //Try base relative addressing
            result = testBaseRelativeAddressing(targetAddress, data, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, true, data);
                return result.second;
            }

            //Try direct addressing
            result = testDirectAddressing(targetAddress, objectCode);
            if(result.first) {
                updateTargetAddressVector(index, false, data);
                return result.second;
            }
        }

        //Address does not fit in a format 3 instruction, convert to format 4
        format = 4;
        instruction->at(2)[0] = '+';
        data->additionalAddressCounter++;
        data->symbolTable->incrementSymbolAddresses(address);
        //Remove element corresponding to this instruction from mustRecalculateObjectCode queue
        //Will be added back in format 4 handling
        mustRecalculateObjectCode->pop_back();
        recalculateInstructionObjectCodes(address, data);
        targetAddress = convertOperandToTargetAddress(instruction->at(3), data).first;
    }
    if(format == 4) {
        //Format 4: opcode (6) + n i x b p e + address (20)
        mustRecalculateObjectCode->push_back(true);

        objectCode = instructionInfo.first;
        objectCode >>= 2;

        //Add first two bits (addressing type)
        objectCode = addBits(objectCode, findAddressingType(instruction->at(3)));

        //Add third bit (x bit)
        if(instruction->at(3).back() == 'X') objectCode = addBits(objectCode, {1});
        else objectCode = addBits(objectCode, {0});

        //Add last 3 bits (b p e); always 0 0 1 because this is format 4 instruction
        objectCode = addBits(objectCode, {0, 0, 1});

        //Add last 20 bits (address)
        objectCode = addNumber(objectCode, targetAddress, 20);
        return objectCode;
    }
    return 0;
}

//Performs all assembling and output processes for one assembly file
void assembleFile(const string& filename) {
    //REMEMBER TO USE .substr(1) ON INSTRUCTION WHEN SEARCHING THROUGH EITHER OF THESE LISTS
    //Initialize a vector containing all assembler directives
    set<string> assemblerDirectives{"START", "END", "RESB", "RESW", "BYTE", "WORD", "BASE",
                                    "NOBASE", "*", "LTORG", "ORG", "EQU", "USE"};
    //Initialize a hashmap to store all instructions, opcodes, and formats
    //Call first for opcode, second for format
    unordered_map<string, pair<int, int>> opTable = createOPTable();

    //Initialize symbol table
    SymbolTable symbolTable;

    //Open source code file
    ifstream sourceFile(filename);
    string line;

    //Initialize data object for ease of passing information to functions
    Data data;
    data.currentAddress = 0;
    data.additionalAddressCounter = 0;
    data.baseRegister = 0;
    data.baseRegisterValid = false;
    data.symbolTable = &symbolTable;
    data.opTable = &opTable;

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

        if(assemblerDirectives.find(lineParts.at(1).substr(1)) != assemblerDirectives.end()) {
            //The current instruction is an assembler directive, must be processed
            if(lineParts.at(1).substr(1) == "LTORG") {
                //LTORG directive should be printed before the literals, so print LTORG first
                instructions.push_back(instruction);

                processAssemblerDirective(&lineParts, &data, &instructions);
            } else {
                processAssemblerDirective(&lineParts, &data, &instructions);

                instructions.push_back(instruction);
            }
        } else {
            //Current instruction is not an assembler directive
            //Check if current instruction contains a label, add it to the symbol table if so
            if(lineParts.at(0) != " ") {
                symbolTable.addSymbol(lineParts.at(0), data.currentAddress, true);
            }

            //Check if current instruction contains a literal, add it to the literal pool if so
            if(lineParts.at(2)[0] == '=') {
                symbolTable.addLiteral(lineParts.at(2));
            }

            instructions.push_back(instruction);

            //Increment address counter
            data.currentAddress += opTable[lineParts.at(1).substr(1)].second;

            if(lineParts.at(1)[0] == '+') data.currentAddress++;
        }
    }

    //Vector containing instructions after making any necessary changes in pass two
    //Vector contains: address, label, instruction, operand, object code
    vector<vector<string>> convertedInstructions;
    //Keeps track of which instructions must be recalculated when format 3 instruction converted to format 4
    vector<bool> mustRecalculateObjectCode;
    data.convertedInstructions = &convertedInstructions;
    data.mustRecalculateObjectCode = &mustRecalculateObjectCode;

    //Pass two of assembler
    //Convert instructions to object code, process certain assembler directives
    for(auto & i : instructions) {
        vector<string> instruction = i;
        vector<string> convertedInstruction;

        //Add first three values to convertedInstruction vector: address, label, instruction (already known)
        unsigned int address = stoi(instruction[0]) + data.additionalAddressCounter;
        string newAddress = convertNumberToHex(address, 4);
        convertedInstruction.push_back(newAddress);
        convertedInstruction.push_back(instruction[1]);
        convertedInstruction.push_back(instruction[2]);

        //Check if the current instruction is a literal definition
        if(instruction.at(1) == "*") {
            convertedInstruction.emplace_back("");
            convertedInstruction.push_back(convertNumberToHex(symbolTable.getLiteralInfo(instruction.at(2))[0], 0));
            convertedInstructions.push_back(convertedInstruction);
            mustRecalculateObjectCode.push_back(false);
            continue;
        }

        //Calculate object code of instruction, or process relevant assembler directives
        if(assemblerDirectives.find(instruction.at(2).substr(1)) == assemblerDirectives.end()) {
            //Current instruction is not an assembler directive, convert instruction to object code and print
            unsigned int objectCode = convertInstructionToObjectCode(&instruction, &data, -1);
            //Update instruction in convertedInstruction in case format was switched to 4
            convertedInstruction[2] = instruction[2];

            //Number of characters displayed in object code depends on format
            int format = opTable.at(instruction.at(2).substr(1)).second;
            if(instruction.at(2)[0] == '+') format++;

            instruction[0] = i[0];

            convertedInstruction.push_back(instruction.at(3));

            //Convert object code to hex with appropriate length and modifiers and add it to the converted instruction
            stringstream convertToHex;
            convertToHex << uppercase << hex << setw(format * 2) << setfill('0') << objectCode;
            string objectCodeInHex = convertNumberToHex(objectCode, format * 2);
            convertedInstruction.push_back(convertToHex.str());
        } else {
            mustRecalculateObjectCode.push_back(false);

            //Check for assembler directives, certain directives must be processed in pass two
            if(instruction.at(2) == " BASE") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data).first;
                data.baseRegister = value;
                data.baseRegisterValid = true;
            }
            if(instruction.at(2) == " NOBASE") {
                data.baseRegisterValid = false;
            }
            if(instruction.at(2) == " END") {
                data.symbolTable->setLengthOfProgram(data.currentAddress + data.additionalAddressCounter);
            }
            convertedInstruction.push_back(instruction.at(3));

            //WORD and BYTE instructions should have their calculated values associated with them
            if(instruction.at(2) == " BYTE") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data).first;
                convertedInstruction.push_back(convertNumberToHex(value, 2));

                //Test if given operand is larger than one byte
                string test = convertNumberToHex(value, 0);
                if(test.length() > 2) {
                    cout << "Error: BYTE assembler directive received operand of size greater than one byte: " << instruction.at(3) << endl;
                    exit(BAD_EXIT);
                }
            } else if(instruction.at(2) == " WORD") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data).first;
                convertedInstruction.push_back(convertNumberToHex(value, 6));

                //Test is given operand is larger than three bytes
                string test = convertNumberToHex(value, 0);
                if(test.length() > 6) {
                    cout << "Error: WORD assembler directive received operand of size greater than one word: " << instruction.at(3) << endl;
                }
            } else {
                convertedInstruction.emplace_back("");
            }
        }

        convertedInstructions.push_back(convertedInstruction);
    }

    //Print out all converted instructions
    //No further processing of instructions done at this stage, only output
    ofstream listingFile;
    string fileWithoutExtension = filename.substr(0, filename.find('.'));

    listingFile.open(fileWithoutExtension + ".l");

    for(auto instruction : convertedInstructions) {
        //Skip printing address on END instruction
        if(instruction[2] == " END") {
            listingFile << "        ";
        } else {
            listingFile << instruction.at(0) << "    ";
        }
        listingFile << instruction.at(1);
        printSpacesToFile(8 - instruction.at(1).length(), &listingFile);
        listingFile << instruction.at(2);
        printSpacesToFile(9 - instruction.at(2).length(), &listingFile);
        listingFile << instruction.at(3);

        printSpacesToFile(26 - instruction.at(3).length(), &listingFile);
        listingFile << instruction.at(4) << endl;
    }

    listingFile.close();

    symbolTable.printSymbols(fileWithoutExtension + ".st");
}

int main(int argc, char** argv) {
    if(argc == 1) {
        cout << "Invalid number of arguments; received 0, expected at least 1." << endl;
        exit(BAD_EXIT);
    }

    for(int i = 1; i < argc; i++) {
        assembleFile(argv[i]);
    }

    return NORMAL_EXIT;
}