#include "Logical.h"
#include "AstVisitor.h"

namespace AST{

Logical::Logical(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    :Expr(token, Type::Bool, constant), expr1(std::move(p1)), expr2(std::move(p2))
{
}

Logical::Logical(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Expr(std::move(token), Type::Bool, constant), expr1(std::move(p1)), expr2(std::move(p2))
{
}

Ast* Logical::accept(AstVisitor & visitor)
{
    return visitor.visitLogical(this);
}

Or::Or(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, std::move(p1), std::move(p2), constant)
{
}

Or::Or(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), std::move(p1), std::move(p2), constant)
{
}

Ast* Or::accept(AstVisitor & visitor)
{
    return visitor.visitOr(this);
}

And::And(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, std::move(p1), std::move(p2), constant)
{
}

And::And(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), std::move(p1), std::move(p2), constant)
{
}

Ast* And::accept(AstVisitor & visitor)
{
    return visitor.visitAnd(this);
}

Not::Not(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, std::move(expr), std::move(expr), constant)
{
}

Not::Not(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), std::move(expr), std::move(expr), constant)
{
}

Ast* Not::accept(AstVisitor & visitor)
{
    return visitor.visitNot(this);
}

Rel::Rel(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, std::move(p1), std::move(p2), constant)
{
}

Rel::Rel(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), std::move(p1), std::move(p2), constant)
{
}

Ast* Rel::accept(AstVisitor & visitor)
{
    return visitor.visitRel(this);
}

// a special case
// In sematics, Odd belongs to Logical and Unary
// maybe we will remove it the next version
Odd::Odd(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, expr, expr, constant)
{
}

Odd::Odd(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), expr, expr, constant)
{
}

Ast* Odd::accept(AstVisitor & visitor)
{
    return visitor.visitOdd(this);
}

}