#include "Logical.h"
#include "AstVisitor.h"

Logical::Logical(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    :Expr(token, Type::Bool, constant), expr1(p1), expr2(p2)
{
}

Logical::Logical(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Expr(std::move(token), Type::Bool, constant), expr1(p1), expr2(p2)
{
}

AstNode Logical::accept(AstVisitor & visitor)
{
    return visitor.visitLogical(static_pointer_cast<Logical>(shared_from_this()));
}

Or::Or(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p2, constant)
{
}

Or::Or(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p2, constant)
{
}

AstNode Or::accept(AstVisitor & visitor)
{
    return visitor.visitOr(static_pointer_cast<Or>(shared_from_this()));
}

And::And(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p2, constant)
{
}

And::And(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p2, constant)
{
}

AstNode And::accept(AstVisitor & visitor)
{
    return visitor.visitAnd(static_pointer_cast<And>(shared_from_this()));
}

Not::Not(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, expr, expr, constant)
{
}

Not::Not(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), expr, expr, constant)
{
}

AstNode Not::accept(AstVisitor & visitor)
{
    return visitor.visitNot(static_pointer_cast<Not>(shared_from_this()));
}

Rel::Rel(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p2, constant)
{
}

Rel::Rel(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p2, constant)
{
}

AstNode Rel::accept(AstVisitor & visitor)
{
    return visitor.visitRel(static_pointer_cast<Rel>(shared_from_this()));
}

Odd::Odd(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, expr, expr, constant)
{
}

Odd::Odd(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), expr, expr, constant)
{
}

AstNode Odd::accept(AstVisitor & visitor)
{
    return visitor.visitOdd(static_pointer_cast<Odd>(shared_from_this()));
}
