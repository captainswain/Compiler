#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(Lexer &lexi, ofstream& file) : 
    lex(lexi),
    m_output(file),
    enablePrint(false),
    instructionTable(),
    symbolTable(),
    declarationMode(false),
    getMode(false),
    assignmentMode(false)
{
    Rat18F();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {}

void SyntaxAnalyzer::printOutput(const std::string message)
{
    if (enablePrint)
        std::cout << message << std::endl;
}

void SyntaxAnalyzer::printError(const std::string error) {
    std::cout << "Error: found '" << getCurrentToken().lexeme << "' expected '" << error << "' near line number " << to_string(lex.getCurrentLine()) << endl;
    std::exit(1); 
}

Token::Wrapper SyntaxAnalyzer::getCurrentToken()
{
    return _currentToken;
}

Token::Wrapper SyntaxAnalyzer::getNextToken()
{
    _currentToken = lex.getToken();
    printOutput("token \t lexeme");
    printOutput(getCurrentToken().toString());
    return _currentToken;
}

// Lexical Conventions
bool SyntaxAnalyzer::Identifier()
{
    if (getCurrentToken().token == Token::Type::IDENTIFIER) {
        printOutput("<Identifier>");
        return true;
    }

    return false;
}

bool SyntaxAnalyzer::Integer()
{
    if (getCurrentToken().token == Token::Type::INTEGER) {
        printOutput("<Integer>");
        return true;
    }

    return false;
}
bool SyntaxAnalyzer::Real()
{
    if (getCurrentToken().token == Token::Type::REAL) {
        printOutput("<Real>");
        return true;
    }

    return false;
}

// Syntax rules

//R1. <Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
bool SyntaxAnalyzer::Rat18F()
{
    getNextToken();
    if (OptFunctionDefinitions()) {
        if (getCurrentToken().lexeme == "$$") {
            if (OptDeclarationList()) {
                if (StatementList()) {
                    if (getCurrentToken().lexeme == "$$") {
                        printOutput("<Rat18F> -> <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$");
                        printOutput("Success! Valid RAT18-F syntax");
                        symbolTable.printTable(m_output);
                        instructionTable.printTable(m_output);
                        return true;
                    } else {
                        printError("$$");
                    }
                }
            }
        } else {
            printError("$$");
        }
    }

    return false;
}

//R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty>
bool SyntaxAnalyzer::OptFunctionDefinitions()
{
    if (FunctionDefintions()) {
        printOutput("<Opt Function Definitions> -> <Function Definitions>");
        return true;
    } else if (Empty()) {
        printOutput("<Opt Function Definitions> -> <Empty>");
        return true;
    }

    return false;
}

//R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions>
bool SyntaxAnalyzer::FunctionDefintions()
{
    if (Function()) {
        if (FunctionDefintions()) {
            printOutput("<Function Definitions>  -> <Function> <Function Definitions>");
            return true;
        } else {
            printOutput("<Function Definitions>  -> <Function>");
            return true;
        }
    }

    return false;
}

//R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>
bool SyntaxAnalyzer::Function()
{
    if (getCurrentToken().lexeme == "function") {
        getNextToken();
        if (Identifier()) {
            if (getNextToken().lexeme == "(") {
                if (OptParameterList()) {
                    if (getCurrentToken().lexeme == ")") {
                        if (OptDeclarationList()) {
                            if (Body()) {
                                getNextToken();
                                printOutput("R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>");
                                return true;
                            }
                        }
                    } else {
                        printError(")");
                    }
                }
            } else {
                printError("(");
            }
        }
    } 
    return false;
}

//R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
bool SyntaxAnalyzer::OptParameterList()
{
    if (ParameterList()) {
        printOutput("<Opt Parameter List> -> <Parameter List> ");
        return true;
    } else if (Empty()) {
        printOutput("<Opt Parameter List> -> <Empty> ");
        return true;
    }

    return false;
}


//R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
bool SyntaxAnalyzer::ParameterList()
{
    if (Parameter()) {
        if (getCurrentToken().lexeme == ",") {
            if (ParameterList()) {
                printOutput("<Parameter List> -> <Parameter>,<Parameter List>");
                return true;
            }
        } else {
            printOutput("<Parameter List>  -> <Parameter>");
            return true;
        }
    }

    return false;
}

//R7. <Parameter> ::=  <IDs > : <Qualifier> 
bool SyntaxAnalyzer::Parameter()
{
    if (IDs()) {
        if (getCurrentToken().lexeme == ":") {
            if (Qualifier()) {
                printOutput("<Parameter> -> <IDs> : <Qualifier>");
                getNextToken();
                return true;
            }
        }
    }
    return false;
}

//R8. <Qualifier> ::= int     |    boolean    |  real 
bool SyntaxAnalyzer::Qualifier()
{
    string str = getNextToken().lexeme;

    if (str == "int") {
        printOutput("<Qualifier> ::= int");
        return true;
    } else if (str == "boolean") {
        printOutput("<Qualifier> ::= boolean");
        return true;
    } else if (str == "real") {
        printOutput("<Qualifier> ::= real");
        return true;
    }

    return false;
}

//R9. <Body>  ::=  {  < Statement List>  }
bool SyntaxAnalyzer::Body()
{
    if (getCurrentToken().lexeme == "{") {
        getNextToken();
        if (StatementList()) {
            if (getCurrentToken().lexeme == "}") {
                printOutput("<Body>  -> {  < Statement List>  }");
                return true;
            } else {
                printError("}");
            }
        }
    } else {
        printError("{");
    }

    return false;
}

//R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
bool SyntaxAnalyzer::OptDeclarationList()
{
    if (DeclarationList()) {
        printOutput("<Opt Declaration List> -> <Declaration List>");
        return true;
    } else if (Empty()) {
        printOutput("<Opt Declaration List> -> <Empty> ");
        return true;
    }
    return false;
}

//R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>
bool SyntaxAnalyzer::DeclarationList()
{
    if (Declaration()) {
        if (getCurrentToken().lexeme == ";") {
            if (DeclarationList()) {
                printOutput("<Declaration List> -> <Declaration>; <Declaration List>");
                return true;
            } else {
                printOutput("<Declaration List>  -> <Declaration> ;");
                return true;
            }
        } else {
            printError(";");
        }
    }

    return false;
}

//R12. <Declaration> ::=   <Qualifier > <IDs>
bool SyntaxAnalyzer::Declaration()
{
    if (Qualifier()) {
        saveToken = getCurrentToken();
        declarationMode = true;        
        // store qualifier (int, real, bool etc.)
        if (IDs()) {
            declarationMode = false;
            // insert
            printOutput("<Declaration> -> <Qualifier > <IDs>");
            return true;
        }
    }

    return false;
}

//R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>
bool SyntaxAnalyzer::IDs()
{
    getNextToken();
    if (Identifier()) {
        if (declarationMode)
            symbolTable.insert(getCurrentToken().lexeme, saveToken.lexeme);   

        if (getMode) {
            if (!symbolTable.identifierExists(getCurrentToken().lexeme)) {
                std::cout << "Error: identifier not defined '" << getCurrentToken().lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
                std::exit(1);
            }
            instructionTable.generateInstruction(InstructionTable::OP::STDIN, -1);
            instructionTable.generateInstruction(InstructionTable::OP::PUSHM, symbolTable.getAddress(getCurrentToken().lexeme));
        }  
        
        if (getNextToken().lexeme == ",") {
            if (IDs()) {
                printOutput("<IDs> -> <Identifier>, <IDs>");
                return true;
            }
        } else {
            printOutput("<IDs> -> <Identifier>");
            return true;
        }
    }

    return false;
}

//R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>
bool SyntaxAnalyzer::StatementList()
{
    if (Statement()) {
        getNextToken();
        if (StatementList()) {
            printOutput("<Statement List> -> <Statement> <Statement List>");
            return true;
        } else {
            printOutput("<Statement List> -> <Statement>");
            return true;
        }
    }

    return false;
}

//R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
bool SyntaxAnalyzer::Statement()
{
    if (Assign()) {
        printOutput("<Statement> -> <Assign>");
        return true;
    } else if (If()) {
        printOutput("<Statement> -> <If>");
        return true;
    } else if (Return()) {
        printOutput("<Statement> -> <Return>");
        return true;
    } else if (Print()) {
        printOutput("<Statement> -> <Print>");
        return true;
    } else if (Scan()) {
        printOutput("<Statement> -> <Scan>");
        return true;
    } else if (While()) {
        printOutput("<Statement> -> <While>");
        return true;
    } else if (Compound()) {
        printOutput("<Statement> -> <Compound>");
        return true;
    }
    return false;
}

//R16. <Compound> ::=   {  <Statement List>  }
bool SyntaxAnalyzer::Compound()
{
    if (getCurrentToken().lexeme == "{") {
        getNextToken();
        if (StatementList()) {
            if (getCurrentToken().lexeme == "}") {
                printOutput("<Compund> -> { <Statement List> }");
                return true;
            } else {
                printError("}");
            }
        }
    }
    return false;
}

//R17. <Assign> ::=     <Identifier> = <Expression> ;
bool SyntaxAnalyzer::Assign()
{
    assignmentMode = true;

    if (Identifier()) {
        saveToken = getCurrentToken();

        // Check to see if identifier in the symbol table if not then throw error
        if (!symbolTable.identifierExists(saveToken.lexeme)) {                  
            std::cout << "Error: identifier not defined '" << saveToken.lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
            std::exit(1);
        }

        if (getNextToken().lexeme == "=") {
            getNextToken();
            if (Expression()) {
                assignmentMode = false;
                instructionTable.generateInstruction(InstructionTable::OP::POPM, symbolTable.getAddress(saveToken.lexeme));
                if (getCurrentToken().lexeme == ";") {
                    printOutput("<Assign> -> <Identifier> = <Expression> ;");
                    return true;
                } else {
                    printError(";");
                }
            }
        } else {
            printError("=");
        }
    }
    return false;
}


// R18. <If> ::=     if  ( <Condition>  ) <Statement>   ifend    |   
//                   if  ( <Condition>  ) <Statement>   else  <Statement>  ifend  
bool SyntaxAnalyzer::If()
{
    if (getCurrentToken().lexeme == "if") {
        saveAddress = instructionTable.getInstructionAddress();
        if (getNextToken().lexeme == "(") {
            getNextToken();
            if (Condition()) {
                if (getCurrentToken().lexeme == ")") {
                    getNextToken();
                    if (Statement()) {
                        getNextToken();
                        if (getCurrentToken().lexeme == "ifend") {
                            instructionTable.backPatch(instructionTable.getInstructionAddress());
                            instructionTable.generateInstruction(InstructionTable::OP::LABEL, -1);
                            printOutput("<if> -> if  ( <Condition>  ) <Statement>   ifend");
                            return true;
                        } else if (getCurrentToken().lexeme == "else") {
                            getNextToken();
                            instructionTable.generateInstruction(InstructionTable::OP::JUMP, -1);
                            instructionTable.backPatch(instructionTable.getInstructionAddress());
                            instructionTable.addJumpAddress(instructionTable.getInstructionAddress() - 1);
                            if (Statement()) {
                                if (getNextToken().lexeme == "ifend") {
                                    instructionTable.backPatch(instructionTable.getInstructionAddress());
                                    instructionTable.generateInstruction(InstructionTable::OP::LABEL, -1);
                                    printOutput("<If> -> if  ( <Condition>  ) <Statement>   else  <Statement>  ifend  ");
                                    return true;
                                } else {
                                    printError("ifend");
                                }
                            }
                        } else {
                            printError("ifend or else");
                        }
                    }
                } else {
                    printError(")");
                }
            }
        } else {
            printError("(");
        }
    }
    return false;
}

//R19. <Return> ::=  return ; |  return <Expression> ;
bool SyntaxAnalyzer::Return()
{
    if (getCurrentToken().lexeme == "return") {
        if (getNextToken().lexeme == ";") {
            printOutput("<Return> ->  return ;");
            return true;
        } else {
            if (Expression()) {
                if (getCurrentToken().lexeme == ";") {
                    printOutput("<Return> -> return <Expression> ;");
                    return true;
                }
                return false;
            }
            return false;
        }
    }
    return false;
}

//R20. <Print> ::=    put ( <Expression>);
//
//
bool SyntaxAnalyzer::Print()
{
    if (getCurrentToken().lexeme == "put") {
        if (getNextToken().lexeme == "(") {
            getNextToken();
            if (Expression()) {
                if (getCurrentToken().lexeme == ")") {
                    if (getNextToken().lexeme == ";") {
                        instructionTable.generateInstruction(InstructionTable::OP::STDOUT, 0);
                        printOutput("<Print> ->  put ( <Expression> ); ");
                        return true;
                    } else {
                        printError(";");
                    }
                } else {
                    printError(")");
                }
            }
        } else {
            printError("(");
        }
    }
    return false;
}

//R21. <Scan> ::=    get ( <IDs> );
bool SyntaxAnalyzer::Scan()
{
    if (getCurrentToken().lexeme == "get") {
        if (getNextToken().lexeme == "(") {
            getMode = true;
            if (IDs()) {
                if (getCurrentToken().lexeme == ")") {
                    if (getNextToken().lexeme == ";") {
                        getMode = false;
                        printOutput("<Scan> -> get( <IDs> );");
                        return true;
                    } else {
                        printError(";");
                    }
                } else {
                    printError(")");
                }
            } else {
                printError("identifier");
            }
        } else {
            printError("(");
        }
    }
    return false;
}

//R22. <While> ::=  while ( <Condition>  )  <Statement>  whileend
bool SyntaxAnalyzer::While()
{
    if (getCurrentToken().lexeme == "while") {
        saveAddress = instructionTable.getInstructionAddress();
        instructionTable.generateInstruction(InstructionTable::OP::LABEL, -1);
        if (getNextToken().lexeme == "(") {
            getNextToken();
            if (Condition()) {
                if (getCurrentToken().lexeme == ")") {
                    getNextToken();
                    if (Statement()) {
                        instructionTable.generateInstruction(InstructionTable::OP::JUMP, saveAddress);
                        instructionTable.backPatch(instructionTable.getInstructionAddress());
                        if (getNextToken().lexeme == "whileend") {
                            printOutput("<While> -> ( <Condition>  )  <Statement>  whileend");
                            return true;
                        } else {
                            printError("whileend");
                        }
                    }
                } else {
                    printError(")");
                }
            } else {
                printError("expression operator expression");
            }
        } else {
            printError("(");
        }
    }
    return false;
}

//R23. <Condition> ::=     <Expression>  <Relop>   <Expression>
bool SyntaxAnalyzer::Condition()
{
    if (Expression()) {
        if (Relop()) {
            std::string op = getCurrentToken().lexeme;
            getNextToken();
            if (Expression()) {
                if (op == "==") {
                    instructionTable.generateInstruction(InstructionTable::OP::EQU, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                } else if (op == "^=") {
                    instructionTable.generateInstruction(InstructionTable::OP::NEQ, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                } else if (op == ">") {
                    instructionTable.generateInstruction(InstructionTable::OP::GRT, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                } else if (op == "<") {
                    instructionTable.generateInstruction(InstructionTable::OP::LES, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                } else if (op == "=>") {
                    instructionTable.generateInstruction(InstructionTable::OP::GEQ, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                } else if (op == "=<") {
                    instructionTable.generateInstruction(InstructionTable::OP::LEQ, -1);
                    instructionTable.addJumpAddress(instructionTable.getInstructionAddress());
                    instructionTable.generateInstruction(InstructionTable::OP::JUMPZ, -1);
                }
                printOutput("<Condition> -> <Expressiom> <Relop> <Expression>");
                return true;
            }
        }
    }
    return false;
}

//R24. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<
bool SyntaxAnalyzer::Relop()
{
    if (getCurrentToken().lexeme == "==") {
        printOutput("<Relop> -> ==");
        return true;
    } else if (getCurrentToken().lexeme == "^=") {
        printOutput("<Relop> -> ^=");
        return true;
    } else if (getCurrentToken().lexeme == ">") {
        printOutput("<Relop> -> >");
        return true;
    } else if (getCurrentToken().lexeme == "<") {
        printOutput("<Relop> -> <");
        return true;
    } else if (getCurrentToken().lexeme == "=>") {
        printOutput("<Relop> -> =>");
        return true;
    } else if (getCurrentToken().lexeme == "=<") {
        printOutput("<Relop> -> =<");
        return true;
    }

    return false;
}

// R25
bool SyntaxAnalyzer::Expression()
{
    if (Term()) {
        // getNextToken();
        if (ExpressionPrime()) {
            printOutput("<Expression> -> <Term><ExpressionPrime>");
            return true;
        }
    }
    return false;
}

//R25B. <ExpressionPrime>  ::= +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon
bool SyntaxAnalyzer::ExpressionPrime()
{
    if (getCurrentToken().lexeme == "+") {
        getNextToken();
        if (Term()) {
            instructionTable.generateInstruction(InstructionTable::OP::ADD, -1);
            if (ExpressionPrime()) {
                printOutput("<ExpressionPrime> -> + <Term><ExpressionPrime>");
                return true;
            }
        }
    }
    else if (getCurrentToken().lexeme == "-") {
        getNextToken();
        if (Term()) {
            instructionTable.generateInstruction(InstructionTable::OP::SUB, -1);
            if (ExpressionPrime()) {
                printOutput("<ExpressionPrime> -> - <Term><ExpressionPrime> | epsilon");
                return true;
            }
        }
    } else if (Empty()) {
        printOutput("<ExpressionPrime> -> Empty");
        return true;
    }

    return false;
}

//R26. <Term>    ::= <Factor><TermPrime>
bool SyntaxAnalyzer::Term()
{
    if (Factor()) {
        if (TermPrime()) {
            printOutput("<Term> -> <Factor><TermPrime>");
            return true;
        }
    }

    return false;
}

//R26B. <TermPrime>    ::= *<Factor><TermPrime> | /<Factor><TermPrime> | epsilon
bool SyntaxAnalyzer::TermPrime()
{
    if (getCurrentToken().lexeme == "*") {
        getNextToken();
        if (Factor()) {
            instructionTable.generateInstruction(InstructionTable::OP::MUL, -1);
            if (TermPrime()) {
                printOutput("<TermPrime> -> *<Factor><TermPrime>");
                return true;
            }
        }
    } else if (getCurrentToken().lexeme == "/") {
        getNextToken();
        if (Factor()) {
            instructionTable.generateInstruction(InstructionTable::OP::DIV, -1);
            if (TermPrime()) {
                printOutput("<TermPrime> -> /<Factor><TermPrime>");
                return true;
            }
        }
    } else if (Empty()) {
        printOutput("<TermPrime> -> epsilon");
        return true;
    }
    return false;
}

//R27. <Factor> ::=      -  <Primary>    |    <Primary>
bool SyntaxAnalyzer::Factor()
{
    if (getCurrentToken().lexeme == "-") {
        getNextToken();
        if (Primary()) {
            printOutput("<Factor> -> -  <Primary>");
            return true;
        }
    } else if (Primary()) {
        printOutput("<factor> -> <Primary>");
        return true;
    }
    return false;
}

//R28. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real>  |   true   |  false
bool SyntaxAnalyzer::Primary()
{
    if (getCurrentToken().lexeme == "true") {
        if (assignmentMode) {
            if (symbolTable.typeCheck(saveToken.lexeme, "boolean"))
            {
                instructionTable.generateInstruction(InstructionTable::OP::PUSHI, 1);
            } else {
                std::cout << "Error: Type mismatch. Cannot assign '" << getCurrentToken().lexeme << "' to '" << saveToken.lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
                std::exit(1);
            }
        }
        printOutput("<Primary> -> true");
        getNextToken();
        return true;
    }
    else if (getCurrentToken().lexeme == "false") {
        if (assignmentMode)
        {
            if (symbolTable.typeCheck(saveToken.lexeme, "boolean")) {
                instructionTable.generateInstruction(InstructionTable::OP::PUSHI, 0);
            } else {
                std::cout << "Error: Type mismatch. Cannot assign '" << getCurrentToken().lexeme << "' to '" << saveToken.lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
                std::exit(1);
            }
        }
        printOutput("<Primary> -> false");
        getNextToken();
        return true;
    }
    else if (Integer()) {
        if (assignmentMode)
        {
            if (symbolTable.typeCheck(saveToken.lexeme, "int")) {
                instructionTable.generateInstruction(InstructionTable::OP::PUSHI, std::stoi(getCurrentToken().lexeme));
            } else {
                std::cout << "Error: Type mismatch. Cannot assign '" << getCurrentToken().lexeme << "' to '" << saveToken.lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
                std::exit(1);
            }
        }
        printOutput("<Primary> -> <Integer>");
        getNextToken();
        return true;
    }
    else if (Real()) {
        printOutput("<Primary> -> <Real>");
        getNextToken();
        return true;
    }
    else if (getCurrentToken().lexeme == "(") {
        getNextToken();
        if (Expression()) {
            if (getCurrentToken().lexeme == ")") {
                printOutput("<Primary> -> ( <Expression> ) ");
                getNextToken();
                return true;
            } else {
                printError(")");
            }
        }
    } else if (Identifier()) {
        // Check to see if identifier in the symbol table if not then throw error
        if (!symbolTable.identifierExists(getCurrentToken().lexeme)) {
            std::cout << "Error: identifier not defined '" << getCurrentToken().lexeme << "' near line number " << to_string(lex.getCurrentLine()) << endl;
            std::exit(1);
        }

        instructionTable.generateInstruction(InstructionTable::OP::PUSHM, symbolTable.getAddress(getCurrentToken().lexeme));
        if (getNextToken().lexeme == "(") {
            if (IDs()) {
                if (getNextToken().lexeme == ")") {
                    printOutput("<Primary> -> <Identifier> ( <IDs> )");
                    return true;
                } else {
                    printError(")");
                }
            }
        } else {
            printOutput("<Primary> -> <Identifier>");
            return true;
        }
    }

    return false;
}

//R29. <Empty>   ::= 
bool SyntaxAnalyzer::Empty()
{
    printOutput("<Empty> -> e");
    return true;
}