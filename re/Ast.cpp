#include "Ast.h"
#include "AstVisitor.h"

using std::static_pointer_cast;

AstNode Program::accept(AstVisitor & visitor)
{
    return visitor.visitProgram(static_pointer_cast<Program>(shared_from_this()));
}

Program::Program(BodyNode body)
    :body(body)
{
}

string Ast::toString()
{
    return string();
}
