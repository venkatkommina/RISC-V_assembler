#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Function to encode assembly into machine code
vector<string> encodeInstructions(const vector<vector<string>>& instructions);

// Function to generate .mc file
void generateMCFile(const vector<vector<string>>& instructions, const vector<string>& machineCode, 
                    const unordered_map<int, string>& dataMemory, const string& filename);

#endif
