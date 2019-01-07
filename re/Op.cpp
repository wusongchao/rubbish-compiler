#include "Op.h"

Op::Op(const CodeToken & token, const Type & type, bool constant)
    :Expr(token, type, constant)
{
}

Op::Op(CodeToken && token, const Type & type, bool constant)
    :Expr(std::move(token), type, constant)
{
}

Arith::Arith(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    :Op(token, Type::Null, constant), expr1(p1), expr2(p2)
{
    type = expr1->type;
}

Arith::Arith(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Op(std::move(token), Type::Null, constant), expr1(p1), expr2(p2)
{
    type = expr1->type;
}

Unary::Unary(const CodeToken & token, ExprNode expr, bool constant)
    :Op(token, expr->type, constant), expr(expr)
{
}

Unary::Unary(CodeToken && token, ExprNode expr, bool constant)
    : Op(std::move(token), expr->type, constant), expr(expr)
{
}
