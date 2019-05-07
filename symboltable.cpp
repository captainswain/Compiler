#include "symboltable.h"

#include <iostream>

// Constructor/deconstructor
SymbolTable::SymbolTable() :
	_current_memory_address(5000)
{}

SymbolTable::~SymbolTable()
{}

bool SymbolTable::identifierExists(std::string identifier)
{
    if (_symtable.find(identifier) == _symtable.end()) 
        return false;
    return true;
}

bool SymbolTable::typeCheck(std::string identifier, std::string type)
{
    if (_symtable.find(identifier) == _symtable.end() || _symtable.at(identifier).type != type)
        return false;

    return true;
}

bool SymbolTable::insert(std::string identifier, std::string type)
{
    SymbolTableEntry temp;
    temp.memoryAddress = _current_memory_address++;
    temp.type = type;
    temp.identifier = identifier;

    if (!identifierExists(identifier))
    {
        _symtable[identifier] = temp;
    } else {
        return false;
    }
     return true;
}

void SymbolTable::printTable(std::ofstream& file)
{
    file << "Identifier \tMemory Location\tType\n";
    for (auto x : _symtable)
        file << x.first << "\t" << x.second.memoryAddress << "\t" << x.second.type << std::endl;
}

int SymbolTable::getAddress(std::string identifier)
{
    return _symtable.at(identifier).memoryAddress;
}