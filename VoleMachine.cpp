#include "VoleMachine.h"

// Register class method
// Constructor: Initializes a vector of 16 registers

Register::Register() : registers(16, "00") {}

// Returns the content of the register at the specified address.
string Register::getCell(int address) {
    return registers[address];
}

// Sets the content of the register at the specified address.
void Register::setCell(int address, string cellContent) {
    if (cellContent.size() < 2)     // leading zeros
        cellContent = '0' + cellContent;
    registers[address] = cellContent;
}

// Memory class methods
// Constructor: Initializes a vector of 256 memory cells
Memory::Memory() : memory(256, "00") {}

// Returns the content of the memory cell at the specified address
string Memory::getCell(int address) {

    if(address < 256) {
        return memory[address];
    }
}

// Sets the content of the memory cell at the specified address
void Memory::setCell(int address, string cellContent) {
    if(address < 256) {
        if (cellContent.size() < 2)
            cellContent = '0' + cellContent;
        memory[address] = cellContent;
    }
}

// ALU class methods
int ALU::hexaToDecimal(string hex) {  // convert  from Hexa to Decimal
    int res = 0;
    int base = 1;

    for (int i = hex.size() - 1; i >= 0; i--) {

        char hexDigit = hex[i];

        if (hexDigit >= '0' && hexDigit <= '9') {
            res += (hexDigit - '0') * base;
        }
        else if (hexDigit >= 'A' && hexDigit <= 'F') {
            res += (hexDigit - 'A' + 10) * base;
        }
        else if (hexDigit >= 'a' && hexDigit <= 'f') {
            res += (hexDigit - 'a' + 10) * base;
        }

        base *= 16;
    }
    return res;
}

string ALU::decimalToHex(int decimal) {    // convert from Decimal to Hexa
    if (decimal == 0) return "0";
    string hex;
    const char hexDigits[] = "0123456789ABCDEF";
    while (decimal > 0) {
        int remainder = decimal % 16;
        hex = hexDigits[remainder] + hex;
        decimal /= 16;
    }
    return hex;
}

string ALU::decimalToBin(int decimal) {   //convert from Decimal to Binary
    if (decimal == 0)
        return "0";
    string binary;
    while (decimal > 0) {
        if(decimal % 2 == 0) {
            binary += '0';
        }
        else {
            binary += '1';
            decimal--;

        }
        decimal /= 2;
    }
    reverse(binary.begin(), binary.end());

    return binary;
}

int ALU::binToDecimal(string binary) {     //convert from  Binary to Decimal
    int decimal = 0;
    reverse(binary.begin(), binary.end());
    for(int i = 0; i < binary.size(); i++) {
        decimal += (binary[i] - '0') * pow(2,i);
    }
    return decimal;
}

int ALU::convert2ComplementToDec(string binary){    //convert from  Two Complement to Decimal
    while (binary.size() < 8){
        binary = '0' + binary;
    }
    if (binary[0] == '1'){
        binary = decimalToBin(binToDecimal(binary) - 1);
        for (int i = 0; i < 8; ++i) {
            if (binary[i] == '1')
                binary[i] = '0';
            else
                binary[i] = '1';
        }
        return -1*binToDecimal(binary);
    }
    return binToDecimal(binary);
}

string ALU::convertDecTo2Complement(int decimal){  //convert from  Decimal  to Two Complement
    string binary;
    if (decimal < 0){
        binary = decimalToBin(-1 * (decimal + 1));
        while (binary.size() < 8){
            binary = '0' + binary;
        }
        for (int i = 0; i < 8; ++i) {
            if (binary[i] == '1')
                binary[i] = '0';
            else
                binary[i] = '1';
        }
    }
    else {
        binary = decimalToBin(decimal);
    }
    return binary;
}

double ALU::convertBinToFloat(string binary){  //convert from Binary to Float
    // Explicit floating-point conversion
    // (-1)^sign * 0.mantissa * 2^(exponent - 4)

    while (binary.size() < 8){
        binary = '0' + binary;
    }
    int sign = (binary[0] == '1') ? -1 : 1;
    string exponent = binary.substr(1, 3);
    string mantissa = binary.substr(4, 4);
    double mantissaFloat = 0;
    int counter = -1;
    for (int i = 0; i < 4; ++i) {
        mantissaFloat += pow(2, counter) * (mantissa[i] - '0');
        counter--;
    }
    int exponentInt = binToDecimal(exponent) - 4;
    double ans = sign * pow(2, exponentInt) * mantissaFloat;
    return ans;
}

string ALU::convertFloatToBin(double floatNum) {
    if (floatNum == 0) return "00000000"; // Zero case

    char sign = (floatNum < 0) ? '1' : '0';
    floatNum = abs(floatNum);

    int exponent = 0;
    while (floatNum < 0.5 && exponent > -4) {
        floatNum *= 2;
        exponent--;
    }

    int biasedExponent = exponent + 4;
    string exponentBin = decimalToBin(biasedExponent);
    while (exponentBin.size() < 3) {
        exponentBin = '0' + exponentBin; // Pad to ensure it’s exactly 3 bits
    }

    string mantissa;
    for (int i = 0; i < 4; ++i) {
        floatNum *= 2;
        if (floatNum >= 1) {
            mantissa += '1';
            floatNum -= 1;
        } else {
            mantissa += '0';
        }
    }
    return sign + exponentBin + mantissa;
}

void ALU::addTwoComplement (int idxReg1, int idxReg2, int idxReg3,Register& reg) {
    string binary1 = decimalToBin(hexaToDecimal(reg.getCell(idxReg2)));
    string binary2 = decimalToBin(hexaToDecimal(reg.getCell(idxReg3)));

    int decimal1 = convert2ComplementToDec(binary1);
    int decimal2 = convert2ComplementToDec(binary2);

    string result = convertDecTo2Complement(decimal1+decimal2);

    while (result.size() < 8){  //lead zeros
        result = '0' + result;
    }
    result = decimalToHex(binToDecimal(result));

    reg.setCell(idxReg1, result);
};

void ALU::addFloatingPoint (int idxReg1, int idxReg2, int idxReg3,Register& reg) {
    string binary1 = decimalToBin(hexaToDecimal(reg.getCell(idxReg2)));
    string binary2 = decimalToBin(hexaToDecimal(reg.getCell(idxReg3)));

    double decimal1 = convertBinToFloat(binary1);
    double decimal2 = convertBinToFloat(binary2);

    string result = decimalToBin(binToDecimal(convertFloatToBin(decimal1 + decimal2)));

    reg.setCell(idxReg1,decimalToHex(binToDecimal(result)));
}

void ALU::OR(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1 = hexaToDecimal(reg.getCell(idxReg1));
    int n2 = hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    while (num1.size() < 8) {  //lead zeros
        num1= '0' + num1;
    }
    while (num2.size() < 8) {  //lead zeros
        num2= '0' + num2;
    }

    for (int i = 0; i < num1.size(); i++) {
        if (num1[i] == '0' &&  num2[i] == '0') {
            result += '0';
        } else {
            result += '1';
        }
    }
    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}


void ALU::AND(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1=hexaToDecimal(reg.getCell(idxReg1));
    int n2=hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    while (num1.size() < 8) { // lead zeros
        num1= '0' + num1;
    }
    while (num2.size() < 8) {  // lead zeros
        num2= '0' + num2;
    }
    for (int i = 0; i < num1.size(); i++) {

        if (num1[i] == '1' && num2[i] == '1') {
            result += '1';
        } else {
            result += '0';
        }
    }

    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}

void ALU::XOR(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1=hexaToDecimal(reg.getCell(idxReg1));
    int n2=hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    while (num1.size() < 8) {  // lead zeros
        num1= '0' + num1;
    }
    while (num2.size() < 8) {  // lead zeros
        num2= '0' + num2;
    }

    for (int i = 0; i < num1.size(); i++) {
        if (num1[i] == num2[i] ) {
            result += '0';
        } else {
            result += '1';
        }
    }

    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}

// CU class methods
// Load function : load content of memory address
void CU::load(int idxReg, int MemAddr, Register& reg, Memory& mem) {
    reg.setCell(idxReg, mem.getCell(MemAddr));
}

// Load function: load value
void CU::load(int idxReg, string val, Register& reg) {
    reg.setCell(idxReg, val);
}

//  Store function : to store in memory
void CU::store(int idxReg, int idxMem, Register& reg, Memory& mem) {
    mem.setCell(idxMem, reg.getCell(idxReg));
}


// Move function: copies the content from one register to another
void CU::move(int idxReg1, int idxReg2, Register& reg) {
    reg.setCell(idxReg2, reg.getCell(idxReg1));
}

// Jump function: updates the program counter if the register(idx) value ==  register(0) value
void CU::jump(int idxReg, int MemAddr, Register& reg, int& PC) {
    if (reg.getCell(idxReg) == reg.getCell(0)) {
        PC = MemAddr;
    }
}

// Rotate function: rotates the binary for content specified register by a given number of steps
void CU::Rotate(int idxReg, int step, ALU& alu, Register& reg) {
    int n = alu.hexaToDecimal(reg.getCell(idxReg));

    string num = alu.decimalToBin(n);

    while (num.size() < 8) {
        num = '0' + num;
    }

    step = step % num.size();

    string Sub = num.substr(num.size() - step, step);
    string result = Sub + num.substr(0, num.size() - step);

    int content = alu.binToDecimal(result);
    string val=alu.decimalToHex(content);

    if (val.size() == 1){
        val = '0' + val;
    }
    reg.setCell(idxReg, val);
}

// Jump2 function: updates the program counter if the two's complement value of
// one register is greater than another
void CU::jump2(int idxReg, int MemAddr, Register& reg, int& PC,ALU& alu) {
    string binary1 = alu.decimalToBin(alu.hexaToDecimal(reg.getCell(idxReg)));
    string binary2 = alu.decimalToBin(alu.hexaToDecimal(reg.getCell(0)));

    int num1 = alu.convert2ComplementToDec(binary1);
    int num2 = alu.convert2ComplementToDec(binary2);
    cout<<binary1<<" " <<binary2<<endl;

    cout<<num1<<" " <<num2<<endl;

    if (num1 > num2){
        PC = MemAddr;
    }
}

void CU::halt() {
    cout << "Program halted." << endl;
}

// CPU class methods
CPU::CPU() : programCounter(10) {}

void CPU::Fetch() {
    cout << "Program Counter = " << programCounter << endl;
    instructionRegister = mem.getCell(programCounter++);
    instructionRegister += mem.getCell(programCounter++);

    cout << "Instruction Register: " << instructionRegister << endl;
}

int CPU::DecodeAndExecute(const string& instruction) {
    char opCode = instruction[0];
    int idxReg1, idxReg2, idxReg3, idxMem;

    if (opCode == '1') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1);

        string hexMemAdd = instruction.substr(2, 2);
        idxMem = alu.hexaToDecimal(hexMemAdd);

        cu.load(idxReg1, idxMem, registers, mem);

        cout << "Loading register " << Reg1 << " with bit pattern found in adress " << hexMemAdd << endl << endl;

    }
    else if (opCode == '2') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1);

        string val = instruction.substr(2, 2);

        cu.load(idxReg1, val, registers);

        cout << "Loading register " << Reg1 << " with bit pattern " << val << endl << endl;

    }
    else if (opCode == '3') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1);

        if (instruction[2] == '0' && instruction[3] == '0') {
            string x = registers.getCell(idxReg1);

            mem.setCell(0, x);
            cout << "Mapping to screen bit pattern found in Register " << Reg1 << ": " << x << endl << endl;
        }

        else {
            string hexMemAdd = instruction.substr(2, 2);
            idxMem = alu.hexaToDecimal(hexMemAdd);

            cu.store(idxReg1, idxMem, registers, mem);

            cout << "Store bit pattern found in register " << Reg1 << " with bit pattern found in " <<
                 hexMemAdd << endl << endl;
        }
    }
    else if (opCode == '4') {
        if (instruction[1] == '0') {
            string Reg1(1, instruction[2]);
            string Reg2(1, instruction[3]);

            idxReg1 = alu.hexaToDecimal(Reg1);
            idxReg2 = alu.hexaToDecimal(Reg2);

            cu.move(idxReg1, idxReg2, registers);

            cout << "Move bit pattern found in register " << Reg1 << " to register " << Reg2 << endl << endl;
        }
    }
    else if (opCode == '5') {
        string Reg1(1, instruction[1]);
        string Reg2(1, instruction[2]);
        string Reg3(1, instruction[3]);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxReg2 = alu.hexaToDecimal(Reg2);
        idxReg3 = alu.hexaToDecimal(Reg3);

        alu.addTwoComplement (idxReg1, idxReg2, idxReg3,registers);


        cout << "Add bit pattern found in register " << Reg2 <<
             " with bit pattern found in register " << Reg3 << " using 2's complement and store result in register "
             << Reg1 << endl << endl;

    }
    else if (opCode == '6') {
        string Reg1(1, instruction[1]);
        string Reg2(1, instruction[2]);
        string Reg3(1, instruction[3]);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxReg2 = alu.hexaToDecimal(Reg2);
        idxReg3 = alu.hexaToDecimal(Reg3);

        alu.addFloatingPoint (idxReg1, idxReg2, idxReg3 ,registers);

        cout << "Add bit pattern found in register " << Reg2 <<
             " with bit pattern found in register " << Reg3 << " using floating point notation and store result in register"
             << Reg1 << endl << endl;
    }

    else if (opCode == '7') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxReg2 = alu.hexaToDecimal(Reg2);
        idxReg3 = alu.hexaToDecimal(Reg3);

        alu.OR(idxReg1, idxReg2, idxReg3,registers);

        cout << "OR operation on bit pattern in register " << Reg1 << " and " <<
             Reg2 << " and store result in register " << Reg3 << endl << endl;
    }

    else if (opCode == '8') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxReg2 = alu.hexaToDecimal(Reg2);
        idxReg3 = alu.hexaToDecimal(Reg3);

        alu.AND(idxReg1, idxReg2, idxReg3,registers);

        cout << "AND operation on bit pattern in register " << Reg1 << " and " <<
             Reg2 << " and store result in register " << Reg3 << endl << endl;
    }
    else if (opCode == '9') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxReg2 = alu.hexaToDecimal(Reg2);
        idxReg3 = alu.hexaToDecimal(Reg3);

        alu.XOR(idxReg1, idxReg2, idxReg3,registers);

        cout << "XOR operation on bit pattern in register " << Reg1 << " and " << Reg2 <<
             " and store result in register " << Reg3 << endl << endl;

    }
    else if (opCode == 'A') {
        if (instruction[2] == '0') {
            string Reg2(1, instruction[3]);
            string Reg3(1, instruction[1]);
            idxReg2 = alu.hexaToDecimal(Reg2);
            idxReg3 = alu.hexaToDecimal(Reg3);

            cu.Rotate(idxReg3, idxReg2 ,alu,registers);

            cout << "Rotate content of register " << Reg3 << " cyclically right " << Reg2 << " steps\n" << endl;
        }

    }
    else if (opCode == 'B') {
        string hexMemAdd = instruction.substr(2, 2);
        string Reg1(1, instruction[1]);

        idxMem = alu.hexaToDecimal(hexMemAdd);
        idxReg1 = alu.hexaToDecimal(Reg1);

        cu.jump(idxReg1, idxMem, registers, programCounter);

        cout << "Jump to memory address: " << hexMemAdd << " if register " << Reg1 <<" have same content of Register 0\n" << endl;

    }
    else if (opCode == 'C') {
        if (instruction[1] == '0' && instruction[2] == '0' && instruction[3] == '0') {
            cu.halt();
        }
    }
    else if (opCode == 'D'){

        string Reg1(1, instruction[1]);
        string hexMemAdd = instruction.substr(2, 2);

        idxReg1 = alu.hexaToDecimal(Reg1);
        idxMem = alu.hexaToDecimal(hexMemAdd);

        cu.jump2(idxReg1, idxMem, registers, programCounter,alu);

        cout << "Jump to memory address: " << hexMemAdd << " if content of register " << Reg1 <<
             " is > content of Register 0 (data are interpreted in 2's complement\n" << endl;
    }
    return programCounter;
}

Memory& CPU::getMemory() {
    return mem;
}

Register& CPU::getRegister() {
    return registers;
}

ALU& CPU::getALU() {
    return alu;
}

// Machine class methods
void Machine::loadProgramFile(string& filename) {
    ifstream inputFile(filename);

    while (!inputFile.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        cout << "Enter the filename again: ";
        cin >> filename;
        inputFile.open(filename);
    }

    string line;
    int address = 10;
    bool first=false;
    bool check =false;

    while (inputFile >> line) {
        if (line.size() == 4 && address < 256) {
            getProcessor().getMemory().setCell(address++, line.substr(0, 2));
            getProcessor().getMemory().setCell(address++, line.substr(2, 2));   // Increment address for the next instruction
            if(getProcessor().getMemory().getCell(address - 2) == "C0"  && getProcessor().getMemory().getCell(address - 1)=="00" ) {
                check=true;
            }
        } else {
            if(!first)
                cout << "overflow instrctions xceeded memory"<< endl;
            first=true;

        }
    }

    inputFile.close();

    // Make sure we are not exceeding memory bounds
    if (address <= 256) {
        if(!check && address < 256) {
            getProcessor().getMemory().setCell(address++, "C0");
            getProcessor().getMemory().setCell(address, "00");

        }
        else if(address==256) {

            address -= 2;
            getProcessor().getMemory().setCell(address++, "C0");
            getProcessor().getMemory().setCell(address, "00");

        }
    }
}


void Machine::outputState() {
    for (int i = 0; i < 16; i++) {
        cout << "Register #" << getProcessor().getALU().decimalToHex(i) << ": " << getProcessor().getRegister().getCell(i) << endl;
    }
    cout << "=============================================\n";
    cout << "Memory content\n";
    cout << "    ";
    for (int i = 0; i < 16; ++i) {
        cout << " " << getProcessor().getALU().decimalToHex(i) << "  ";
    }
    cout << endl;
    for (int i = 0; i < 16; ++i) {
        cout << getProcessor().getALU().decimalToHex(i) << " | ";
        for (int j = 0; j < 16; ++j) {
            cout << " " << getProcessor().getMemory().getCell(i*16+j ) << " ";
        }
        cout << endl;
    }
}

CPU& Machine::getProcessor() {
    return processor;
}

// MainUI class methods
void MainUI::DisplayMenu() {    // Menu Program VoleMachine
    cout << "===== Machine Simulator Menu =====" << endl;
    cout << "1. Load Program File" << endl;
    cout << "2. Enter Instructions" << endl;
    cout << "3. Output State" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

string MainUI::inputFileName() {
    cout << "Enter program file name: ";
    string filename;
    cin >> filename;
    return filename;
}

string MainUI::inputInstruction() {
    cout << "Enter an instruction: ";
    string instruction;
    cin >> instruction;

    return  instruction;
}

string MainUI::inputChoice() {
    string choice;
    cin >> choice;
    return choice;
}

void MainUI::run() {
    CPU cpu;
    bool exit = false;
    bool stop = false;

    while (true) {
        DisplayMenu();
        string choice = inputChoice();

        if (choice == "1") {
            string filename = inputFileName();
            machine.loadProgramFile(filename);

            cout << "What do you want?\n"
                    "1) Run instruction per time.\n"
                    "2) Run the whole file" << endl;

            string choice2;
            cin >> choice2;

            while (choice2 != "1" && choice2 != "2") {
                cout << "Invalid Choice. Please enter 1 or 2" << endl;
                cin >> choice2;
            }
            if (choice2 == "1") {
                string instruction = machine.getProcessor().getMemory().getCell(cpu.programCounter++);
                instruction += machine.getProcessor().getMemory().getCell(cpu.programCounter++);

                machine.getProcessor().Fetch();

                cpu.programCounter = machine.getProcessor().DecodeAndExecute(instruction);

            } else if (choice2 == "2") {

                while (!exit) {
                    string instruction = machine.getProcessor().getMemory().getCell(cpu.programCounter++);
                    instruction += machine.getProcessor().getMemory().getCell(cpu.programCounter++);

                    machine.getProcessor().Fetch();

                    cpu.programCounter = machine.getProcessor().DecodeAndExecute(instruction);

                    if (instruction == "C000") {
                        exit = true;
                        break;
                    }
                }
            }
        } else if (choice == "2") {

            string instruction = inputInstruction();
            machine.getProcessor().getMemory().setCell(cpu.programCounter++, instruction.substr(0, 2));
            machine.getProcessor().getMemory().setCell(cpu.programCounter++, instruction.substr(2, 2));

            cpu.programCounter -= 2;
            machine.getProcessor().getMemory().getCell(cpu.programCounter++);
            machine.getProcessor().getMemory().getCell(cpu.programCounter++);

            if (instruction == "C000" && stop == false) {
                stop = true;
                machine.getProcessor().Fetch();
                cpu.programCounter = machine.getProcessor().DecodeAndExecute(instruction);

            }
            if (instruction != "C000" && !stop) {
                machine.getProcessor().Fetch();
                cpu.programCounter = machine.getProcessor().DecodeAndExecute(instruction);

            }
        }
        else if (choice == "3") {
            machine.outputState();
        }
        else if (choice == "4") {
            break;
        }
        else {
            cout << "Invalid input. Please try again." << endl;
        }
    }
}