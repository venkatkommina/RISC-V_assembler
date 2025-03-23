#include "encoder.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <unordered_map>
#include<algorithm>

using namespace std;

unordered_map<string, int> registerMap = {
    {"x0", 0}, {"x1", 1}, {"x2", 2}, {"x3", 3}, {"x4", 4},
    {"x5", 5}, {"x6", 6}, {"x7", 7}, {"x8", 8}, {"x9", 9},
    {"x10", 10}, {"x11", 11}, {"x12", 12}, {"x13", 13}, {"x14", 14},
    {"x15", 15}, {"x16", 16}, {"x17", 17}, {"x18", 18}, {"x19", 19},
    {"x20", 20}, {"x21", 21}, {"x22", 22}, {"x23", 23}, {"x24", 24},
    {"x25", 25}, {"x26", 26}, {"x27", 27}, {"x28", 28}, {"x29", 29},
    {"x30", 30}, {"x31", 31}
};

string toBinary(int num, int bits) {
    return bitset<32>(num & ((1 << bits) - 1)).to_string().substr(32 - bits);
}

string encodeRType(const vector<string>& instruction, string opcode, string funct3, string funct7) {
    string rd = toBinary(registerMap[instruction[1]], 5);
    string rs1 = toBinary(registerMap[instruction[2]], 5) ;
    string rs2 = toBinary(registerMap[instruction[3]], 5) ;
    string imm= "NULL";

    return funct7+rs2+rs1+funct3+rd+opcode;
}

string encodeIType(const vector<string>& instruction, string opcode, string funct3) {
    string rd = (instruction.size() > 1) ? toBinary(registerMap[instruction[1]], 5) : "NULL";
    
    string immStr = instruction[2];
    string rs1Str = "NULL", imm = "NULL";

    if (immStr.find("(") != string::npos) {  
        size_t start = immStr.find("(");
        size_t end = immStr.find(")");

        string immValStr = immStr.substr(0, start);
        rs1Str = immStr.substr(start + 1, end - start - 1);
        
        imm = bitset<12>(stoi(immValStr) & 0xFFF).to_string();
    } else {
        rs1Str = (instruction.size() > 2) ? toBinary(registerMap[instruction[2]], 5) : "NULL";
        imm = (instruction.size() > 3) ? bitset<12>(stoi(instruction[3]) & 0xFFF).to_string() : "NULL";
    }
    
    return imm + rs1Str + funct3 + rd + opcode;
}

string encodeSType(const vector<string>& instruction, string opcode, string funct3) {
    string rs1 = (instruction.size() > 2) ? toBinary(registerMap[instruction[2]], 5) : "NULL";
    string rs2 = (instruction.size() > 1) ? toBinary(registerMap[instruction[1]], 5) : "NULL";
    string imm = (instruction.size() > 3) ? toBinary(stoi(instruction[3]), 12) : "NULL";

    return imm.substr(0, 7) + rs2 + rs1 + funct3 + imm.substr(7, 5) + opcode;
}

string encodeSBType(const vector<string>& instruction, string opcode, string funct3) {
    string rs1 = (instruction.size() > 1) ? toBinary(registerMap[instruction[1]], 5) : "NULL";
    string rs2 = (instruction.size() > 2) ? toBinary(registerMap[instruction[2]], 5) : "NULL";
    string imm = (instruction.size() > 3) ? toBinary(stoi(instruction[3]) / 2, 13) : "NULL";

    return imm[0] + imm.substr(2, 6) + rs2 + rs1 + funct3 + imm.substr(8, 4) + imm[1] + opcode;
}

string encodeUJType(const vector<string>& instruction, string opcode) {
    string rd = (instruction.size() > 1) ? toBinary(registerMap[instruction[1]], 5) : "NULL";
    string imm = (instruction.size() > 2) ? toBinary(stoi(instruction[2]), 21) : "NULL";

    return imm[0] + imm.substr(10, 10) + imm[9] + imm.substr(1, 8) + rd + opcode;
}

string encodeUType(const vector<string>& instruction, string opcode) {
    string rd = (instruction.size() > 1) ? toBinary(registerMap[instruction[1]], 5) : "NULL";
    string imm = (instruction.size() > 2) ? toBinary(stoi(instruction[2]), 20) : "NULL";

    return imm + rd + opcode;  // imm[31:12] + rd + opcode
}

vector<string> encodeInstructions(const vector<vector<string>>& instructions) {
    vector<string> machineCode;

    for (const auto& instruction : instructions) {
        string opcode, funct3, funct7, encoded;
        string instr = instruction[0];

        if (instr == "add" || instr == "sub" || instr == "sll" || instr == "slt" || instr == "sra" ||
            instr == "srl" || instr == "xor" || instr == "or" || instr == "and" || instr == "mul" ||
            instr == "div" || instr == "rem") {
            opcode = "0110011";
            funct3 = (instr == "add" || instr == "sub") ? "000" :
                     (instr == "sll") ? "001" :
                     (instr == "slt") ? "010" :
                     (instr == "xor") ? "100" :
                     (instr == "srl" || instr == "sra") ? "101" :
                     (instr == "or") ? "110" :
                     "111";
            funct7 = (instr == "sub") ? "0100000" : "0000000";
            encoded = encodeRType(instruction, opcode, funct3, funct7);
        } 
        else if (instr == "addi" || instr == "andi" || instr == "ori" || instr == "lb" || instr == "ld" || instr == "lh" || instr == "lw" || instr == "jalr") {
            opcode = (instr == "jalr") ? "1100111" : "0010011";
            funct3 = (instr == "addi") ? "000" :
                     (instr == "andi") ? "111" :
                     (instr == "ori") ? "110" :
                     "010";
            encoded = encodeIType(instruction, opcode, funct3);
        }
        else if (instr == "sb" || instr == "sh" || instr == "sw" || instr == "sd") {
            opcode = "0100011";
            funct3 = "010";
            encoded = encodeSType(instruction, opcode, funct3);
        }
        else if (instr == "beq" || instr == "bne" || instr == "bge" || instr == "blt") {
            opcode = "1100011";
            funct3 = (instr == "beq") ? "000" :
                     (instr == "bne") ? "001" :
                     (instr == "blt") ? "100" :
                     "101";
            encoded = encodeSBType(instruction, opcode, funct3);
        }
        else if (instr == "lui" || instr == "auipc") {
            opcode = (instr == "lui") ? "0110111" : "0010111";
            encoded = encodeUType(instruction, opcode);  // Use new U-type function
        }
        else if (instr == "jal") {
            opcode = "1101111";
            encoded = encodeUJType(instruction, opcode);
        }
        else {
            cerr << "Error: Unknown instruction `" << instr << "`" << endl;
            continue;
        }

        machineCode.push_back(encoded);
    }

    return machineCode;
}

void generateMCFile(const vector<vector<string>>& instructions, const vector<string>& machineCode, 
    const unordered_map<int, string>& dataMemory, const string& filename) {
    ofstream mcFile(filename);
    if (!mcFile) {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }

    int pc = 0x00000000;
    for (size_t i = 0; i < instructions.size(); i++) {
        unsigned int binaryValue = stoul(machineCode[i], nullptr, 2);
        stringstream ss;
        ss << hex << uppercase << setw(8) << setfill('0') << binaryValue;
        string machineCodeHex = "0x" + ss.str();

        // Extract fields for breakdown: opcode-funct3-funct7-rd-rs1-rs2-immediate
        string opcode = machineCode[i].substr(25, 7);  // Bits 6:0
        string funct3 = (instructions[i][0] == "lui" || instructions[i][0] == "auipc" || instructions[i][0] == "jal") 
                        ? "NULL" : machineCode[i].substr(12, 3);  // Bits 14:12
        string funct7 = (instructions[i][0] == "add" || instructions[i][0] == "sub" || instructions[i][0] == "sll" ||
                         instructions[i][0] == "slt" || instructions[i][0] == "sra" || instructions[i][0] == "srl" ||
                         instructions[i][0] == "xor" || instructions[i][0] == "or" || instructions[i][0] == "and" ||
                         instructions[i][0] == "mul" || instructions[i][0] == "div" || instructions[i][0] == "rem")
                        ? machineCode[i].substr(0, 7) : "NULL";  // Bits 31:25
        string rd = (instructions[i][0] == "sb" || instructions[i][0] == "sw" || instructions[i][0] == "sd" ||
                     instructions[i][0] == "sh" || instructions[i][0] == "beq" || instructions[i][0] == "bne" ||
                     instructions[i][0] == "bge" || instructions[i][0] == "blt")
                    ? "NULL" : machineCode[i].substr(20, 5);  // Bits 11:7
        string rs1 = (instructions[i][0] == "lui" || instructions[i][0] == "auipc" || instructions[i][0] == "jal")
                     ? "NULL" : machineCode[i].substr(15, 5);  // Bits 19:15
        string rs2 = (instructions[i][0] == "add" || instructions[i][0] == "sub" || instructions[i][0] == "sll" ||
                      instructions[i][0] == "slt" || instructions[i][0] == "sra" || instructions[i][0] == "srl" ||
                      instructions[i][0] == "xor" || instructions[i][0] == "or" || instructions[i][0] == "and" ||
                      instructions[i][0] == "mul" || instructions[i][0] == "div" || instructions[i][0] == "rem" ||
                      instructions[i][0] == "sb" || instructions[i][0] == "sw" || instructions[i][0] == "sd" ||
                      instructions[i][0] == "sh" || instructions[i][0] == "beq" || instructions[i][0] == "bne" ||
                      instructions[i][0] == "bge" || instructions[i][0] == "blt")
                     ? machineCode[i].substr(10, 5) : "NULL";  // Bits 24:20

        string imm;
        if (instructions[i][0] == "addi" || instructions[i][0] == "andi" || instructions[i][0] == "ori" ||
            instructions[i][0] == "lb" || instructions[i][0] == "ld" || instructions[i][0] == "lh" ||
            instructions[i][0] == "lw" || instructions[i][0] == "jalr") {
            imm = machineCode[i].substr(0, 12);  // imm[11:0]
        } else if (instructions[i][0] == "sb" || instructions[i][0] == "sw" || instructions[i][0] == "sd" ||
                   instructions[i][0] == "sh") {
            imm = machineCode[i].substr(0, 7) + machineCode[i].substr(20, 5);  // imm[11:5|4:0]
        } else if (instructions[i][0] == "beq" || instructions[i][0] == "bne" || instructions[i][0] == "bge" ||
                   instructions[i][0] == "blt") {
            imm = machineCode[i].substr(0, 1) + machineCode[i].substr(1, 6) + machineCode[i].substr(21, 4) +
                  machineCode[i].substr(24, 1);  // imm[12|10:5|4:1|11]
        } else if (instructions[i][0] == "lui" || instructions[i][0] == "auipc") {
            imm = machineCode[i].substr(0, 20);  // imm[31:12]
        } else if (instructions[i][0] == "jal") {
            imm = machineCode[i].substr(0, 1) + machineCode[i].substr(1, 10) + machineCode[i].substr(11, 1) +
                  machineCode[i].substr(12, 8);  // imm[20|10:1|11|19:12]
        } else {
            imm = "NULL";
        }

        string binaryBreakdown = opcode + "-" + funct3 + "-" + funct7 + "-" + rd + "-" + rs1 + "-" + rs2 + "-" + imm;

        mcFile << "0x" << hex << setw(1) << pc << " " << machineCodeHex << " , ";
        for (size_t j = 0; j < instructions[i].size(); j++) {
            mcFile << instructions[i][j];
            if (j < instructions[i].size() - 1) mcFile << ",";
        }
        mcFile << " # " << binaryBreakdown << endl;
        pc += 4;
    }

    mcFile << "0x" << hex << setw(1) << pc << " 0xFFFFFFFF , # Program End\n" << endl;

    mcFile << "\n# Data Segment\n";
    vector<pair<int, string>> sortedData(dataMemory.begin(), dataMemory.end());
    sort(sortedData.begin(), sortedData.end());
    for (const auto& entry : sortedData) {
        mcFile << "0x" << hex << setw(8) << setfill('0') << entry.first << " 0x"
               << setw(8) << setfill('0') << stoul(entry.second, nullptr, 10) << endl;
    }

    mcFile.close();
}