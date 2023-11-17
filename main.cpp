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

    output = str.substr(0, lastNonSpaceCharacter + 1);

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

//Checks if the given string is a number or not
bool isStringANumber(string str) {
    istringstream iss(str);
    int num;
    iss >> num;

    return iss.eof() && !iss.fail();
}

//Takes an operand (immediate operand, label, etc.) and converts it to target address
unsigned int convertOperandToTargetAddress(const string& operand, Data* data) {
    //In case of ',X' being present in operand, ignore it
    string shortenedOperand = operand.substr(0, operand.find(','));
    //Isolate first character because it usually indicates what type of operand this is
    char firstChar = shortenedOperand[0];

    //Check if operand is a number
    if(isStringANumber(shortenedOperand)) return stoi(shortenedOperand);

    if(firstChar == '=') {
        //Operand is a literal, get address from symbol table
        return data->symbolTable->getLiteralInfo(shortenedOperand).at(1);
    }
    //Check if operand is an expression
    string operations = "+-*/";
    for(char op : operations) {
        int index = shortenedOperand.find(op);
        if(index != string::npos) {
            //The string contains the operation being checked
            string operand1 = shortenedOperand.substr(0, index);
            string operand2 = shortenedOperand.substr(index + 1);

            //Convert string operands into numbers
            //If the operand is a number, treat it as a number; otherwise, treat it as a symbol
            unsigned int convertedOperand1, convertedOperand2;
            //Keep track of the number of relative terms (certain operations can't be performed with certain number of relative terms)
            int numOfRelative = 0;

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
            //TODO: Perform error checking based on number of relative terms
            if(op == '+') {
                return convertedOperand1 + convertedOperand2;
            }
            if(op == '-') {
                return convertedOperand1 - convertedOperand2;
            }
            if(op == '*') {
                return convertedOperand1 - convertedOperand2;
            }
            if(op == '/') {
                return convertedOperand1 / convertedOperand2;
            }
        }
    }

    if((shortenedOperand[1] == 'C' || shortenedOperand[1] == 'X') && shortenedOperand[2] == '\'') {
        //This is an operand of format X'F1' or C'EOF'
        //Use SymbolTable function to find its value
        return data->symbolTable->getValue(shortenedOperand);
    }
    if(shortenedOperand == "*") return data->currentAddress;
    if(firstChar == '#') {
        //Immediate addressing
        //Check if symbol table contains the operand, if it doesn't, assume the operand is a number
        pair<int, bool> symbolInfo = data->symbolTable->getSymbolInfo(shortenedOperand.substr(1));

        if(symbolInfo.first == -1) {
            return stoi(shortenedOperand.substr(1));
        } else {
            return symbolInfo.first;
        }
    }
    if(firstChar == ' ' || firstChar == '@') {
        //Simple/Indirect addressing, get symbol address from symbol table and return
        return data->symbolTable->getSymbolInfo(shortenedOperand.substr(1)).first;
    }
}

//Process assembler directives, updating address counter and symbol table as necessary
void processAssemblerDirective(vector<string>* lineParts, Data* data, vector<vector<string>>* instructions) {
    string label = lineParts->at(0);
    string instruction = lineParts->at(1).substr(1);
    string operand = lineParts->at(2);
    int targetAddress;
    if(!operand.empty()) targetAddress = convertOperandToTargetAddress(operand, data);

    int address = data->currentAddress;
    SymbolTable* symbolTable = data->symbolTable;

    if(instruction == "START") {
        data->currentAddress = stoi(operand, nullptr, 16);
        data->symbolTable->setCSECT(label, stoi(operand));
    }
    if(instruction == "END") {
        //End of program, call command to pool literals at the current address
        data->symbolTable->setLengthOfProgram(data->currentAddress + 3);
        data->currentAddress = data->symbolTable->setLiteralsAtAddress(data->currentAddress, instructions);
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
        symbolTable->setLiteralsAtAddress(data->currentAddress, instructions);
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
pair<bool, unsigned int> testBaseRelativeAddressing(unsigned int targetAddress, int base, unsigned int objectCode) {
    int diff = targetAddress - base;
    if(base != -1 && diff >= 0 && diff <= 4095) {
        //Meets conditions for base relative addressing, use base relative addressing
        objectCode = addBits(objectCode, {1, 0, 0});
        //Add last 12 bits (displacement), masking not necessary because diff must be positive
        objectCode = addNumber(objectCode, diff, 12);
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

//Processes given instruction and returns object code
//Vector of size 4 stores instruction information: address, label, instruction, operand
unsigned int convertInstructionToObjectCode(vector<string>* instruction, Data* data, unordered_map<string, pair<int, int>>* opTable) {
    pair<int, int> instructionInfo = opTable->at(instruction->at(2).substr(1));
    int format = instructionInfo.second;
    unsigned int objectCode;

    unsigned int targetAddress;

    //Don't calculate target address for format 3/4 instructions
    if(format == 3 || format == 4)  {
        if(instruction->at(2) == " RSUB") return 0x4F0000;
        else targetAddress = convertOperandToTargetAddress(instruction->at(3), data);
    }

    //Maps each register to an int array corresponding to its number (used in format 2)
    unordered_map<char, vector<int>> registerNumbers = initializeRegisterNumbers();

    //Check for '+' before instruction, switch to format 4 if found
    if(instruction->at(2)[0] == '+') format = 4;

    if(format == 1) {
        //Format 1: object code = opcode
        return instructionInfo.first;
    } else if(format == 2) {
        //Format 2: object code = opcode (8 bits) + r1 (4 bits) + r2 (4 bits)
        objectCode = instructionInfo.first;
        objectCode = addBits(objectCode, registerNumbers[instruction->at(3)[1]]);
        objectCode = addBits(objectCode, registerNumbers[instruction->at(3)[3]]);
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
            if(result.first) return result.second;

            //Try PC relative addressing
            result = testPCRelativeAddressing(targetAddress, address, objectCode);
            if(result.first) return result.second;

            //Try base relative addressing
            result = testBaseRelativeAddressing(targetAddress, data->baseRegister, objectCode);
            if(result.first) return result.second;
        } else {
            //Simple/indirect addressing
            //Addressing mode order: PC relative, base relative, direct

            //Try PC relative addressing
            pair<bool, unsigned int> result = testPCRelativeAddressing(targetAddress, address, objectCode);
            if(result.first) return result.second;

            //Try base relative addressing
            result = testBaseRelativeAddressing(targetAddress, data->baseRegister, objectCode);
            if(result.first) return result.second;

            //Try direct addressing
            result = testDirectAddressing(targetAddress, objectCode);
            if(result.first) return result.second;
        }

        //Address does not fit in a format 3 instruction, convert to format 4
        format = 4;
        instruction->at(2)[0] = '+';
    }
    if(format == 4) {
        //Format 4: opcode (6) + n i x b p e + address (20)
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
}

//Helper function to convert a number to a string in hex format
string convertNumberToHex(unsigned int number, int requiredLength) {
    stringstream stream;
    stream << uppercase << hex << setw(requiredLength) << setfill('0') << number;
    return stream.str();
}

int main(int argc, char** argv) {
    if(argc == 1) {
        cout << "Invalid number of arguments; received 0, expected at least 1." << endl;
        exit(BAD_EXIT);
    }

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
            if(!lineParts.at(0).empty()) {
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
    //Contains pairs representing the target address of each instruction in convertedInstructions
    //bool marks if the targetAddress refers to an actual address or if it's just a number
    vector<pair<unsigned int, bool>> targetAddresses;

    //Pass two of assembler
    //Convert instructions to object code, print to output file
    for(int i = 0; i < instructions.size(); i++) {
        vector<string> instruction = instructions.at(i);
        vector<string> convertedInstruction;
        convertedInstruction.push_back(instruction[0]);
        convertedInstruction.push_back(instruction[1]);
        convertedInstruction.push_back(instruction[2]);

        //Check if the current instruction is a literal definition
        if(instruction.at(1) == "*") {
            convertedInstruction.emplace_back("");
            convertedInstruction.push_back(convertNumberToHex(symbolTable.getLiteralInfo(instruction.at(2))[0], 0));
            convertedInstructions.push_back(convertedInstruction);
            continue;
        }

        //Calculate object code of instruction, or process relevant assembler directives
        if(assemblerDirectives.find(instruction.at(2).substr(1)) == assemblerDirectives.end()) {
            //Current instruction is not an assembler directive, convert instruction to object code and print
            instruction[0] = instructions.at(i + 1)[0];
            unsigned int objectCode = convertInstructionToObjectCode(&instruction, &data, &opTable);

            //Number of characters displayed in object code depends on format
            int format = opTable.at(instruction.at(2).substr(1)).second;
            if(instruction.at(2)[0] == '+') format++;

            instruction[0] = instructions.at(i)[0];

            convertedInstruction.push_back(instruction.at(3));

            //Convert object code to hex with appropriate length and modifiers and add it to the converted instruction
            stringstream convertToHex;
            convertToHex << uppercase << hex << setw(format * 2) << setfill('0') << objectCode;
            string objectCodeInHex = convertNumberToHex(objectCode, format * 2);
            convertedInstruction.push_back(convertToHex.str());
        } else {
            //Check for assembler directives, certain directives must be processed in pass two
            if(instruction.at(2) == " BASE") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data);
                data.baseRegister = value;
            }
            if(instruction.at(2) == " NOBASE") {
                data.baseRegister = -1;
            }
            convertedInstruction.push_back(instruction.at(3));

            //WORD and BYTE instructions should have their calculated values associated with them
            //TODO: Error checking for values assigned being too large (over one byte/3 bytes)
            if(instruction.at(2) == " BYTE") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data);
                convertedInstruction.push_back(convertNumberToHex(value, 2));
            } else if(instruction.at(2) == " WORD") {
                unsigned int value = convertOperandToTargetAddress(instruction.at(3), &data);
                convertedInstruction.push_back(convertNumberToHex(value, 6));
            } else {
                convertedInstruction.emplace_back("");
            }
        }

        convertedInstructions.push_back(convertedInstruction);
    }

    //Print out all converted instructions
    //No further processing of instructions done at this stage, only output
    for(auto instruction : convertedInstructions) {
        //Skip printing address on END instruction
        if(instruction[2] == " END") {
            cout << "        ";
        } else {
            cout << uppercase << hex << setw(4) << setfill('0') << stoi(instruction.at(0)) << "    ";
        }
        cout << instruction.at(1);
        printSpaces(8 - instruction.at(1).length());
        cout << instruction.at(2);
        printSpaces(9 - instruction.at(2).length());
        cout << instruction.at(3);

        printSpaces(26 - instruction.at(3).length());
        cout << instruction.at(4) << endl;
    }

    cout << endl << endl;
    symbolTable.printSymbols();

    return NORMAL_EXIT;
}