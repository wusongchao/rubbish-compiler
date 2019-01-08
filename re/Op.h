#pragma once

#include "Expr.h"

class Op : public Expr {
public:
    Op(const CodeToken& token, const Type& type, bool constant = false);

    Op(CodeToken&& token, const Type& type, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

using OpNode = shared_ptr<Op>;

class Arith : public Op {
public:
    Arith(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);

    Arith(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    ExprNode expr1;
    ExprNode expr2;

    AstNode accept(AstVisitor& visitor) override;
};

using ArithNode = shared_ptr<Arith>;

class Unary : public Op {
public:
    Unary(const CodeToken& token, ExprNode expr, bool constant = false);

    Unary(CodeToken&& token, ExprNode expr, bool constant = false);

    ExprNode expr;

    AstNode accept(AstVisitor& visitor) override;
};

using UnaryNode = shared_ptr<Unary>;