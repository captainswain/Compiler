#include <iostream>
#include <string>
#include <fstream>

#include "lexer.h"
#include "syntaxanalyzer.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <file>" << endl;
		return 0;
	}

	ifstream inputFile(argv[1]);
	if (!inputFile.is_open()) {
		cout << "Failed to open input file: " << argv[1] << endl;
		exit(EXIT_FAILURE);
	}

	ofstream outFile("output");
	if (!outFile.is_open()) {
		cout << "Failed to open output file: output" << endl;
		exit(EXIT_FAILURE);
	}

	Lexer lexer(inputFile);

	SyntaxAnalyzer sal(lexer, outFile);

	inputFile.close();
	outFile.close();

	return 0;
}
