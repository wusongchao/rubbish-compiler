#include "AstVisitor.h"

AstNode AstVisitor::visit(AstNode astNode)
{
    if (astNode == nullptr) {
        return astNode;
    }
    return astNode->accept(*this);
}
