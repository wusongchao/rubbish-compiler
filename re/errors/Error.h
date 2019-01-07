#pragma once
#include <exception>
#include <string>

using std::exception;
using std::string;

class CompileError : public exception {
public:
    CompileError(const string& message);
    CompileError(string&& message);

    string info;
};

class SyntaxError : public CompileError
{
public:
    SyntaxError(const string& message);
    SyntaxError(string&& message);
};

class SemanticError : public CompileError
{
public:
    SemanticError(const string& message);
    SemanticError(string&& message);
};