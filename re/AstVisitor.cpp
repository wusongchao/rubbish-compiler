#include "AstVisitor.h"

Ast* AstVisitor::visit(Ast* ast)
{
    if (ast == nullptr) {
        return ast;
    }
    return ast->accept(*this);
}
