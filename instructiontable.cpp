#include "instructiontable.h"

#include <iostream>

InstructionTable::InstructionTable() :
    instructionAddress(1),
    table(),
    jumpStack()
{}

InstructionTable::~InstructionTable() {}

void InstructionTable::generateInstruction(OP op, int oprand)
{
    Entry entry;
    entry.address = getInstructionAddress();
    entry.op = op;
    entry.oprand = oprand;

    table.push_back(entry);
    instructionAddress++;
}

int InstructionTable::getInstructionAddress() const
{
    return instructionAddress;
}

void InstructionTable::addJumpAddress(int jumpAddress)
{
    jumpStack.push(jumpAddress);
}

void InstructionTable::backPatch(int jumpAddress)
{
    int address = jumpStack.top() - 1;
    jumpStack.pop();
    table.at(address).oprand = jumpAddress;
}

void InstructionTable::printTable(std::ofstream& file)
{
    file << "Address\tOp code\tOperand" << std::endl;
    for (auto &entry : table)
    {
        file << entry.address << "\t" << entry.enumVal()  << "\t" << entry.oprand << std::endl;
    }
}