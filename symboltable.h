#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <fstream>

class SymbolTable
{

public:
	SymbolTable();
	~SymbolTable();

	bool identifierExists(std::string identifier);
	bool typeCheck(std::string identifier, std::string type);
	bool insert(std::string identifier, std::string type);
	int getAddress(std::string identifier);
	void printTable(std::ofstream& file);

private:
	struct SymbolTableEntry
	{
		int memoryAddress;
		std::string identifier;
		std::string type;
	};
	unsigned int _current_memory_address;
	std::unordered_map<std::string, SymbolTableEntry> _symtable;
};

#endif