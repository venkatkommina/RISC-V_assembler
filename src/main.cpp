#include <iostream>
#include <vector>

#include "parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./assembler input.asm" << endl;
        return 1;
    }

    string inputFile = argv[1];

    // Parse the Assembly File
    vector<vector<string>> parsedInstructions = parseAssembly(inputFile);

    // Print
    cout << "Parsed Assembly Instructions:" << endl;
    for (const auto& instruction : parsedInstructions) {
        for (const auto& token : instruction) {
            cout << token << endl;
        }
        cout << endl;
    }

    return 0;
}
