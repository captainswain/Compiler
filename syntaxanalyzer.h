#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H


#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "token.h"
#include "lexer.h"
#include "instructiontable.h"
#include "symboltable.h"

class SyntaxAnalyzer
{

public:
	SyntaxAnalyzer(Lexer& lexi, ofstream& file);
	~SyntaxAnalyzer();

private:
	bool enablePrint;
	bool assignmentMode;
	bool getMode;
	bool declarationMode;

	Token::Wrapper _currentToken;
	Token::Wrapper _nextToken;
	
	// Lexer
	Lexer &lex;

	// Instruction Table
	InstructionTable instructionTable;

	// Symbol Table
	SymbolTable symbolTable;
	Token::Wrapper saveToken;
	int saveAddress;

	// Outfile
	ofstream& m_output;

	// Helper Functions
	void printOutput(const std::string message);
	void printError(const std::string error);

	Token::Wrapper getNextToken();
	Token::Wrapper getCurrentToken();

	// Lexical Conventions
	bool Identifier();
	bool Integer();
	bool Real();

	// Syntax rules
	bool Rat18F();                 // R1
	bool OptFunctionDefinitions(); // R2
	bool FunctionDefintions();     // R3
	bool Function();               // R4
	bool OptParameterList();       // R5
	bool ParameterList();          // R6
	bool Parameter();              // R7
	bool Qualifier();              // R8
	bool Body();                   // R9
	bool OptDeclarationList();     // R10
	bool DeclarationList();        // R11
	bool Declaration();            // R12
	bool IDs();                    // R13
	bool StatementList();          // R14
	bool Statement();              // R15
	bool Compound();               // R16
	bool Assign();                 // R17
	bool If();                     // R18
	bool Return();                 // R19
	bool Print();                  // R20
	bool Scan();                   // R21
	bool While();                  // R22
	bool Condition();              // R23
	bool Relop();                  // R24
	bool Expression();             // R25
	bool ExpressionPrime();        // R25-B
	bool Term();                   // R26
	bool TermPrime();              // R26-B
	bool Factor();                 // R27
	bool Primary();                // R28
	bool Empty();                  // R29
};

#endif