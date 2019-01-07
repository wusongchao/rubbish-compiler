#include "Expr.h"
#include "AstVisitor.h"

using std::make_shared;
using std::to_string;

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

Id::Id(const CodeToken & token, const Type& type, int offset, bool constant)
    :Expr(token, type, constant), offset(offset)
{
}

Id::Id(CodeToken && token, const Type& type, int offset, bool constant)
    :Expr(std::move(token), type, constant), offset(offset)
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

shared_ptr<Constant> Constant::createInteger(int i)
{
    return make_shared<Constant>(
        CodeToken{CodeTokenType::Integer, to_string(i)}, Type::Int
        );
}

shared_ptr<Constant> Constant::createBool(bool b)
{
    if (b == true) {
        return make_shared<Constant>(
            CodeToken{ CodeTokenType::True, "true" }, Type::Bool
            );
    } else {
        return make_shared<Constant>(
            CodeToken{ CodeTokenType::False, "false" }, Type::Bool
            );
    }

}
