#pragma once
#include <exception>
#include <string>

using std::exception;
using std::string;

class CompileError : exception {
public:
    CompileError(const string& message);
    CompileError(string&& message);

    string info;
};

class SyntaxError : CompileError
{
public:
    SyntaxError(const string& message);
    SyntaxError(string&& message);
};

class SemanticError : CompileError
{
public:
    SemanticError(const string& message);
    SemanticError(string&& message);
};