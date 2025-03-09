#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<string>> parseAssembly(const string& filename, unordered_map<string, int>& labelTable, unordered_map<int, string>& dataMemory) {
    ifstream file(filename);
    vector<vector<string>> instructions;
    string line;

    int code_address = 0x00000000;   // Code segment starts at 0x00000000
    int data_address = 0x10000000;   // Data segment starts at 0x10000000
    int* current_address = &code_address; // Start in the code segment

    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        return instructions;
    }

    while (getline(file, line)) {
        // empty lines and comments
        if (line.empty() || line[0] == '#') continue;  

        stringstream ss(line);
        vector<string> tokens;
        string word;

        while (ss >> word) {
            if (word[0] == '#') break;
            word.erase(remove(word.begin(), word.end(), ','), word.end()); // Remove commas
            tokens.push_back(word);
        }

        if (!tokens.empty()) {
            // Handle segment directives
            if (tokens[0] == ".text") {
                current_address = &code_address; // Switch to code segment
                continue;  // not add this to instructions
            }
            if (tokens[0] == ".data") {
                current_address = &data_address; // Switch to data segment
                continue;  // not add this to instructions
            }

            //label
            if (tokens[0].back() == ':') {
                string label = tokens[0].substr(0, tokens[0].size() - 1); // :
                labelTable[label] = *current_address; //Store label with the current memory address
                tokens.erase(tokens.begin()); //Remove the label from the tokens
            }

            //data section
            if(*current_address >= 0x10000000) {
                if (tokens.empty()) continue;

                string directive = tokens[0];

                if (directive == ".word") {
                    for (size_t i = 1; i < tokens.size(); i++) {
                        dataMemory[*current_address] = tokens[i];
                        *current_address += 4; //4 bytes ahead
                    }
                } 
                else if (directive == ".half") {
                    for (size_t i = 1; i < tokens.size(); i++) {
                        dataMemory[*current_address] = tokens[i];
                        *current_address += 2; //2 bytes ahead
                    }
                } 
                else if (directive == ".byte") {
                    for (size_t i = 1; i < tokens.size(); i++) {
                        dataMemory[*current_address] = tokens[i];
                        *current_address += 1; //1 byte ahead
                    }
                } 
                else if (directive == ".asciiz") {
                    string asciiString = line.substr(line.find('"') + 1); // Extract string after "
                    asciiString = asciiString.substr(0, asciiString.rfind('"')); // Remove ending "

                    for (char ch : asciiString) {
                        dataMemory[*current_address] = to_string(ch); //each char as byte
                        *current_address += 1;
                    }
                    dataMemory[*current_address] = "0"; // Store null terminator
                    *current_address += 1;
                }
            }
            else{
                //instruction
                if (!tokens.empty()) {
                    instructions.push_back(tokens);
                    *current_address += 4;  // Increment address ONLY for actual instructions
                }
            }
        }
    }

    file.close();
    return instructions;
}

void replaceLabels(vector<vector<string>>& instructions, unordered_map<string, int>& labelTable) {
    int pc = 0x00000000;

    for (auto& instruction : instructions) {
        if (instruction.size() >= 2) {
            string& opcode = instruction[0];  // instruction type

            // Handling branch instructions (beq, bne, blt, bge)
            if ((opcode == "beq") || (opcode == "bne") || (opcode == "blt") || (opcode == "bge")) {
                string label = instruction[3];  //label is at index 3

                if (labelTable.find(label) != labelTable.end()) {
                    int targetAddress = labelTable[label];  // Get label address
                    int offset = (targetAddress - pc);  // "offset"

                    instruction[3] = to_string(offset);  // Replace label with offset
                } else {
                    cerr << "Error: Undefined label '" << label << "' used in " << opcode << " instruction." << endl;
                }
            }

            // Handling jump instructions (jal)
            else if (opcode == "jal") {
                string label = instruction[2];  //label is at index 2 (jal x1, target)

                if (labelTable.find(label) != labelTable.end()) {
                    int targetAddress = labelTable[label];  // Get label address
                    int offset = (targetAddress - pc);  //"offset"

                    instruction[2] = to_string(offset);  // Replace label with offset
                } else {
                    cerr << "Error: Undefined label '" << label << "' used in jal instruction." << endl;
                }
            }

            else if (opcode == "jalr") {
                string& immediate = instruction[3];  
                if (labelTable.find(immediate) != labelTable.end()) {
                    instruction[3] = to_string(labelTable[immediate]);  // Replace label with actual address
                }
            }
        }
        pc += 4;  // Move to the next instruction
    }
}
