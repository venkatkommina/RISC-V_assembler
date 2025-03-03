#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<string>> parseAssembly(const string& filename, unordered_map<string, int>& labelTable) {
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

            //instruction
            if (!tokens.empty()) {
                instructions.push_back(tokens);
                *current_address += 4;  // Increment address ONLY for actual instructions
            }
        }
    }

    file.close();
    return instructions;
}
