#include "AstVisitor.h"

void AstVisitor::visit(shared_ptr<Ast> astNode)
{
    astNode->accept(*this);
}
