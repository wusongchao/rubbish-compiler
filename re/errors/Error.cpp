#include "Error.h"

SyntaxError::SyntaxError(const char* message)
    :exception(message)
{
}
