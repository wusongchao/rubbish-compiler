#include "Op.h"
#include "AstVisitor.h"

namespace AST{

Op::Op(const CodeToken & token, const Type & type, bool constant)
    :Expr(token, type, constant)
{
}

Op::Op(CodeToken && token, const Type & type, bool constant)
    :Expr(std::move(token), type, constant)
{
}

Ast* Op::accept(AstVisitor & visitor)
{
    return visitor.visitOp(this);
}

Arith::Arith(const CodeToken & token, ExprNode p1, ExprNode p2, bool constant)
    :Op(token, Type::Null, constant), expr1(std::move(p1)), expr2(std::move(p2))
{
    type = expr1->type;
}

Arith::Arith(CodeToken && token, ExprNode p1, ExprNode p2, bool constant)
    : Op(std::move(token), Type::Null, constant), expr1(std::move(p1)), expr2(std::move(p2))
{
    type = expr1->type;
}

Ast* Arith::accept(AstVisitor & visitor)
{
    return visitor.visitArith(this);
}

Unary::Unary(const CodeToken & token, ExprNode expr, bool constant)
    :Op(token, expr->type, constant), expr(std::move(expr))
{
}

Unary::Unary(CodeToken && token, ExprNode expr, bool constant)
    : Op(std::move(token), expr->type, constant), expr(std::move(expr))
{
}

Ast* Unary::accept(AstVisitor & visitor)
{
    return visitor.visitUnary(this);
}

}