#include "lexer.h"
#include "token.h"

#include <vector>

const array<string, 14> KEYWORDS = {"if", "else", "ifend", 
                                    "while", "whileend",
                                    "int", "boolean", "real",
                                    "function", "put", "get",
                                    "return", "true", "false"};

const array<string, 11> OPERATORS = {"==", ">", "<",
                                     "=>", "=<", "^=",
                                     "+", "-", "*",
                                     "/", "="};

const array<string, 8> SEPARATORS = {"(", ")", ",",
                                     "{", "}", ";",
                                     ":", "$$"};

/*
	Integer and Real number Finite state machine
	Accepting state: 2 (integer), 3 (real)
*/
int intRealFSM[5][3] = {{1, 4, 4},
                        {1, 2, 4},
                        {3, 4, 4},
                        {3, 4, 4},
                        {4, 4, 4}};

/*
	Identifier Finite state machine
	Accepting state: 2
*/
int identifierFSM[5][3] = {{1, 4, 4},
                           {2, 3, 4},
                           {2, 3, 4},
                           {2, 3, 4},
                           {4, 4, 4}};

int intRealCharToCol(char c)
{
    return c == '.' ? 1 : isdigit(c) ? 0 : 2;
}

Token::Type intRealDFSM(string sequence)
{
    int state = 0;

    for (size_t i = 0; i < sequence.size(); ++i)
    {
        int col = intRealCharToCol(sequence.at(i));
        state = intRealFSM[state][col];
    }

    return state == 1 ? Token::Type::INTEGER : state == 3 ? Token::Type::REAL : Token::Type::UNKNOWN;
}

int identifierCharToCol(const char &c)
{
    return isalpha(c) ? 0 : isdigit(c) ? 1 : 2;
}

Token::Type identifierDFSM(string sequence)
{
    int state = 0;

    for (size_t i = 0; i < sequence.size(); ++i)
    {
        int col = identifierCharToCol(sequence.at(i));
        state = identifierFSM[state][col];
    }

    return state == 1 || state == 2 ? Token::Type::IDENTIFIER : Token::Type::UNKNOWN;
}

Lexer::Lexer(ifstream &file) :
    m_input(file),
    m_currentLine(1),
    m_c(),
    m_buffer("") {}

Lexer::~Lexer() {}

Token::Wrapper Lexer::getToken() {
    m_buffer = "";

    stripChars();
    m_input.get(m_c);

    if (isalpha(m_c)) {
        while (!m_input.eof() && !isspace(m_c) && !isToken(string(1, m_c), SEPARATORS) && !isToken(string(1, m_c), OPERATORS)) {
            m_buffer += m_c;
            m_input.get(m_c);
        }

        m_input.unget();

        if (isToken(m_buffer, KEYWORDS))
            return Token::Wrapper(Token::Type::KEYWORD, m_buffer, m_currentLine);
        else
            return Token::Wrapper(identifierDFSM(m_buffer), m_buffer, m_currentLine);
    }
    else if (isdigit(m_c)) {
        while (!m_input.eof() && !isspace(m_c) && !isToken(string(1, m_c), SEPARATORS) && !isToken(string(1, m_c), OPERATORS)) {
            m_buffer += m_c;
            m_input.get(m_c);
        }

        m_input.unget();

        return Token::Wrapper(intRealDFSM(m_buffer), m_buffer, m_currentLine);
    }
    else if (isToken(string(1, m_c), SEPARATORS)) {
        return Token::Wrapper(Token::Type::SEPARATOR, string(1, m_c), m_currentLine);
    }
    else {
        while (!m_input.eof() && !isspace(m_c) && !isToken(string(1, m_c), SEPARATORS) && !isToken(string(1, m_c), OPERATORS)) {
            m_buffer += m_c;
            m_input.get(m_c);
        }

        if (isToken(string(1, m_c), OPERATORS)) {
            while (!m_input.eof() && !isspace(m_c) && !isalpha(m_c) && !isdigit(m_c) && !isToken(string(1, m_c), SEPARATORS)) {
                m_buffer += m_c;
                m_input.get(m_c);
            }
            m_input.unget();
            return Token::Wrapper(Token::Type::OPERATOR, m_buffer, m_currentLine);
        }
        else if (isToken(m_buffer, SEPARATORS))
            return Token::Wrapper(Token::Type::SEPARATOR, m_buffer, m_currentLine);
        else
            return Token::Wrapper(Token::Type::UNKNOWN, m_buffer, m_currentLine);
    }
}

int Lexer::getCurrentLine() const {
    return m_currentLine;
}

void Lexer::stripChars() {
    while (m_input.get(m_c)) {
        if ((int) m_c == 10)
            m_currentLine++;
        else if (m_c == '[' && m_input.peek() == '*') {
            while (m_input.get(m_c)) {
                if ((int) m_c == 10)
                    m_currentLine++;
                else if (m_c == ']')
                    break;
            }
        }
        else if (!isspace(m_c)) {
            m_input.unget();
            break;
        }
    }
}

template<size_t SIZE>
bool Lexer::isToken(const string &lexeme, const array<string, SIZE> &arr) {
    for (auto &token : arr)
        if (token == lexeme)
            return true;

    return false;
}
