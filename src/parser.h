#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Function prototype for parsing assembly file
vector<vector<string>> parseAssembly(const string& filename, unordered_map<string, int>& labels, unordered_map<int, string>& dataMemory);

void replaceLabels(vector<vector<string>>& instructions, unordered_map<string, int>& labelTable);

#endif
