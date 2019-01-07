#include "Expr.h"
#include "AstVisitor.h"

Expr::Expr(const CodeToken & token, const Type& type, bool constant)
    :token(token), type(type), constant(constant)
{
}

Expr::Expr(CodeToken && token, const Type& type, bool constant)
    :token(std::move(token)), type(type), constant(constant)
{
}

void Expr::accept(AstVisitor & visitor)
{
    return visitor.visitExpr(static_pointer_cast<Expr>(shared_from_this()));
}

Id::Id(const CodeToken & token, const Type& type, bool constant)
    :Expr(token, type, constant)
{
}

Id::Id(CodeToken && token, const Type& type, bool constant)
    :Expr(std::move(token), type, constant)
{
}

Constant::Constant(const CodeToken & token, const Type& type)
    :Expr(token, type, true)
{
}

Constant::Constant(CodeToken && token, const Type& type)
    :Expr(std::move(token), type, true)
{
}
