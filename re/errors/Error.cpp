#include "Error.h"

CompileError::CompileError(const string & message)
    :info(message)
{
}

CompileError::CompileError(string && message)
    :info(std::move(message))
{
}

SyntaxError::SyntaxError(const string & message)
    :CompileError(message)
{
}

SyntaxError::SyntaxError(string && message)
    :CompileError(std::move(message))
{
}

SemanticError::SemanticError(const string & message)
    :CompileError(message)
{
}

SemanticError::SemanticError(string && message)
    :CompileError(std::move(message))
{
}
