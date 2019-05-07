#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <array>

#include "token.h"

using namespace std;

class Lexer {

private:
    ifstream& m_input;
    int m_currentLine;
    char m_c;
    string m_buffer;

private:
    void stripChars();

    template <size_t SIZE>
    bool isToken(const string &lexeme, const array<string, SIZE> &arr);

public:
    Lexer(ifstream& file);
    ~Lexer();

    Token::Wrapper getToken();
    int getCurrentLine() const;

};

#endif //LEXER_H
