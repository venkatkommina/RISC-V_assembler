#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>

#include "parser.h"
#include "encoder.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./assembler <input.asm>" << endl;
        return 1;
    }

    string inputFile = argv[1];

    unordered_map<string, int> labelTable;
    unordered_map<int, string> dataMemory;

    vector<vector<string>> parsedInstructions = parseAssembly(inputFile, labelTable, dataMemory);
    cout << "Assembly parsed successfully." << endl;

    replaceLabels(parsedInstructions, labelTable);
    cout << "Labels replaced successfully." << endl;

    vector<pair<int, string>> sortedData(dataMemory.begin(), dataMemory.end());
    sort(sortedData.begin(), sortedData.end());

    cout << "\nLabel Table (Symbol Table):" << endl;
    for (const auto& entry : labelTable) {
        cout << entry.first << " -> 0x" << hex << setfill('0') << setw(8) << entry.second << endl;
    }

    cout << "\nData Segment (Memory Layout):" << endl;
    for (const auto& entry : sortedData) {
        cout << "0x" << setw(8) << setfill('0') << hex << uppercase << entry.first
             << " : " << entry.second << endl;
    }

    cout << "\nParsed Assembly Instructions:" << endl;
    for (const auto& instruction : parsedInstructions) {
        for (const auto& token : instruction) {
            cout << token << " ";
        }
        cout << endl;
    }

    vector<string> machineCode = encodeInstructions(parsedInstructions);
    cout << "Instructions encoded successfully." << endl;

    string outputFile = "output.mc";
    generateMCFile(parsedInstructions, machineCode, dataMemory, outputFile);

    ifstream checkFile(outputFile);
    if (checkFile.good()) {
        cout << "File " << outputFile << " generated successfully." << endl;
    } else {
        cerr << "Error: File " << outputFile << " was NOT created!" << endl;
    }
    checkFile.close();

    return 0;
}
