#include "Expr.h"
#include "AstVisitor.h"

Expr::Expr(const CodeToken & token, Type type)
    :token(token), type(type)
{
}

Expr::Expr(const CodeToken & token, Type type, bool constant)
    :token(token), type(type), constant(constant)
{
}

void Expr::accept(AstVisitor & visitor)
{
    return visitor.visitExpr(static_pointer_cast<Expr>(shared_from_this()));
}

Id::Id(const CodeToken & token, Type type)
    :Expr(token, type)
{
}

Id::Id(const CodeToken & token, Type type, bool constant)
    :Expr(token, type, constant)
{
}
