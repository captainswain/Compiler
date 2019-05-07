cmake_minimum_required(VERSION 3.0.0)
project(Compiler VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Compiler main.cpp lexer.cpp lexer.h syntaxanalyzer.cpp syntaxanalyzer.h symboltable.cpp symboltable.h instructiontable.cpp instructiontable.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
