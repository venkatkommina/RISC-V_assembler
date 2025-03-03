#include <iostream>
#include <vector>

#include <iomanip>

#include "parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./assembler input.asm" << endl;
        return 1;
    }

    string inputFile = argv[1];

    unordered_map<string, int> labelTable;  // Store labels and addresses

    // Parse the Assembly File
    vector<vector<string>> parsedInstructions = parseAssembly(inputFile, labelTable);

    cout << "Label Table (Symbol Table):" << endl;
    for (const auto& entry : labelTable) {
        cout << entry.first << " -> 0x" << hex << setfill('0') << setw(8) << entry.second << endl;
    }


    // Print
    cout << "\nParsed Assembly Instructions:" << endl;
    for (const auto& instruction : parsedInstructions) {
        for (const auto& token : instruction) {
            cout << token << " ";
        }
        cout << endl;
    }
    return 0;
}
