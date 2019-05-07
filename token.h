#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace Token {
    enum class Type
    {
        UNKNOWN,
        INTEGER,
        REAL,
        IDENTIFIER,
        OPERATOR,
        SEPARATOR,
        KEYWORD
    };

    struct Wrapper
    {
        Type token;
        std::string lexeme;
        int lineNumber;

        Wrapper() : 
            token(),
            lexeme(),
            lineNumber() {}

        Wrapper(Type tok, std::string str, int linenum) : 
            token(tok),
            lexeme(str),
            lineNumber(linenum) {}

        std::string enumVal()
        {
            switch (token)
            {
            case Type::UNKNOWN:    return "Unknown";
            case Type::INTEGER:    return "Integer";
            case Type::REAL:       return "Real";
            case Type::IDENTIFIER: return "Identifier";
            case Type::OPERATOR:   return "Operator";
            case Type::SEPARATOR:  return "Separator";
            case Type::KEYWORD:    return "Keyword";
            default: return "Unknown";
            }
        }

        std::string toString()
        {
            return enumVal() + "  \t" + lexeme + "\t\tline number: " + std::to_string(lineNumber);
        }
    };
}

#endif //TOKEN_H