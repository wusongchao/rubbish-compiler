#include "Error.h"

SyntaxError::SyntaxError(const char* message)
    :exception(message)
{
}

SemanticError::SemanticError(const char * message)
    :exception(message)
{
}
