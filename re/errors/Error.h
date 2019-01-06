#pragma once
#include <exception>

using std::exception;

class SyntaxError : exception
{
public:
    SyntaxError(const char* message);
};