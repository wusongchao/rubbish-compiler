#include "Ast.h"
#include "AstVisitor.h"

using std::static_pointer_cast;

void Program::accept(AstVisitor & visitor)
{
    return visitor.visitProgram(static_pointer_cast<Program>(shared_from_this()));
}
