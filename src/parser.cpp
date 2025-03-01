#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<vector<string>> parseAssembly(const string& filename) {
    ifstream file(filename);
    vector<vector<string>> instructions;
    string line;

    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        return instructions;
    }

    while (getline(file, line)) {
        //empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        vector<string> tokens;
        string word;

        // Tokenize the line
        while (ss >> word) {
            //commas
            word.erase(remove(word.begin(), word.end(), ','), word.end());
            tokens.push_back(word);
        }

        if (!tokens.empty()) {
            instructions.push_back(tokens);
        }
    }

    file.close();
    return instructions;
}
