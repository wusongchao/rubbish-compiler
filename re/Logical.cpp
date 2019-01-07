#include "Logical.h"

Logical::Logical(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    :Expr(token, Type::Bool, constant)
{
}

Logical::Logical(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Expr(std::move(token), Type::Bool, constant)
{
}

Or::Or(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p2, constant)
{
}

Or::Or(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p2, constant)
{
}

And::And(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p2, constant)
{
}

And::And(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p2, constant)
{
}

Not::Not(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, expr, expr, constant)
{
}

Not::Not(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), expr, expr, constant)
{
}

Rel::Rel(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(token, p1, p1, constant)
{
}

Rel::Rel(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Logical(std::move(token), p1, p1, constant)
{
}

Odd::Odd(const CodeToken & token, ExprNode expr, bool constant)
    : Logical(token, expr, expr, constant)
{
}

Odd::Odd(CodeToken && token, ExprNode expr, bool constant)
    : Logical(std::move(token), expr, expr, constant)
{
}
