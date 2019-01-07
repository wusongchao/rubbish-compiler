#include "AstVisitor.h"

AstNode AstVisitor::visit(AstNode astNode)
{
    return astNode->accept(*this);
}
