#ifndef VOLEMACHINESIMULATOR_VOLEMACHINE_H
#define VOLEMACHINESIMULATOR_VOLEMACHINE_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

class Register {
private:
    vector<string> registers;
public:
    Register();
    string getCell(int address);
    void setCell(int address, string cellContent);
};

class Memory {
private:
    vector<string> memory;
public:
    Memory();
    string getCell(int address);
    void setCell(int address, string cellContent);
};

class ALU {
public:

    int hexaToDecimal(string hex);
    int binToDecimal(string binary);
    int convert2ComplementToDec(string binary);

    string decimalToHex(int decimal);
    string decimalToBin(int decimal);
    string convertDecTo2Complement(int decimal);
    string convertFloatToBin(double floatNum);

    void addTwoComplement (int idxReg1, int idxReg2, int idxReg3,Register& reg);
    void addFloatingPoint (int idxReg1, int idxReg2, int idxReg3,Register& reg);
    void OR(int idxReg1, int idxReg2, int idxReg3, Register& reg);
    void AND(int idxReg1, int idxReg2, int idxReg3, Register& reg);
    void XOR(int idxReg1, int idxReg2, int idxReg3, Register& reg);

    double convertBinToFloat(string binary);

};

class CU {
public:
    void load(int idxReg, int MemAddr, Register& reg, Memory& mem);
    void load(int idxReg, string val, Register& reg);
    void store(int idxReg, int idxMem, Register& reg, Memory& mem);
    void move(int idxReg1, int idxReg2, Register& reg);
    void jump(int idxReg, int MemAddr, Register& reg, int& PC);
    void jump2(int idxReg, int MemAddr, Register& reg, int& PC,ALU &alu);

    void Rotate(int idxReg, int step, ALU& alu, Register& reg);
    void halt();
};

class CPU {
private:
    ALU alu;
    CU cu;
    Register registers;
    Memory mem;
    string instructionRegister;

public:
    int programCounter;

    CPU();
    void Fetch();
    int DecodeAndExecute(const string& instruction);
    Memory& getMemory();
    Register& getRegister();
    ALU& getALU();
};

class Machine {
private:
    CPU processor;
public:
    void loadProgramFile(string& filename);
    void outputState();
    CPU& getProcessor();
};

class MainUI {
private:
    Machine machine;

public:
    void DisplayMenu();
    string inputFileName();
    string inputInstruction();
    string inputChoice();
    void run();
};

#endif //VOLEMACHINE_H
