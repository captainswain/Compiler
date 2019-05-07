#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

#include <vector>
#include <string>
#include <stack>
#include <fstream>

class InstructionTable
{

public:
    enum class OP
    {
        PUSHI,  // {Integer Value} Pushes the {Integer Value} onto the Top of the Stack (TOS)
        PUSHM,  // {ML - Memory Location} Pushes the value stored at {ML} onto TOS
        POPM,   // {ML} Pops the value from the top of the stack and stores it at {ML}}
        STDOUT, // Pops the value from TOS and outputs it to the standard output
        STDIN,  // Get the value from the standard input and place in onto the TOS
        ADD,    // Pop the first two items from stack and push the sum onto the TOS
        SUB,    // Pop the first two items from stack and push the difference onto the TOS ( Second item - First item)
        MUL,    // Pop the first two items from stack and push the product onto the TOS
        DIV,    // Pop the first two items from stack and push the result onto the TOS ( Second item /  First item and ignore the remainder)
        GRT,    // Pops two items from the stack and pushes 1 onto TOS if second item is larger otherwise push 0
        LES,    // Pops two items from the stack and pushes 1 onto TOS if the second item is smaller than first item otherwise push 0
        EQU,    // Pops two items from the stack and pushes 1 onto TOS if they are equal otherwise push 0
        NEQ,    // Pops two items from the stack and pushes 1 onto TOS if they are not equal Ottherwise push 0
        GEQ,    // Pops two items from the stack and pushes 1 onto TOS if second item is larger or equal, otherwise push 0
        LEQ,    // Pops two items from the stack and pushes 1 onto TOS if second item is less or equal, otherwise push 0
        JUMPZ,  // {IL - Instruction Location} Pop the stack and  if the value is 0 then jump to {IL}
        JUMP,   // {IL} Unconditionally jump to {IL}
        LABEL   // Empty Instruction;  Provides the instruction location to jump to.
    };

    InstructionTable();
    ~InstructionTable();

    void generateInstruction(OP op, int oprand);
    int getInstructionAddress() const;
    void addJumpAddress(int jumpAddress);
    void backPatch(int jumpAddress);
    void printTable(std::ofstream& file);

private:
    struct Entry {
        int address;
        InstructionTable::OP op;
        int oprand;

        std::string enumVal()
        {
            switch (op)
            {
            case OP::PUSHI:     return "PUSHI";
            case OP::PUSHM:     return "PUSHM";
            case OP::POPM:      return "POPM";
            case OP::STDOUT:    return "STDOUT";
            case OP::STDIN:     return "STDIN";
            case OP::ADD:       return "ADD";
            case OP::SUB:       return "SUB";
            case OP::MUL:       return "MUL";
            case OP::DIV:       return "DIV";
            case OP::GRT:       return "GRT";
            case OP::LES:       return "LES";
            case OP::EQU:       return "EQU";
            case OP::NEQ:       return "NEQ";
            case OP::GEQ:       return "GEQ";
            case OP::LEQ:       return "LEQ";
            case OP::JUMPZ:     return "JUMPZ";
            case OP::JUMP:      return "JUMP";
            case OP::LABEL:     return "LABEL";                                
            default:            return "Unknown";
            }
        }
    };

    std::vector<Entry> table;
    std::stack<int> jumpStack;
    int instructionAddress;
};

#endif
