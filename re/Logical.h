#pragma once
#include "Expr.h"

class Logical : public Expr{
public:
    Logical(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Logical(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;

    ExprNode expr1, expr2;
};

class Or : public Logical {
public:
    Or(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Or(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

class And : public Logical {
public:
    And(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    And(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

class Not : public Logical {
public:
    Not(const CodeToken& token, ExprNode expr, bool constant = false);
    Not(CodeToken&& token, ExprNode expr, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

class Rel : public Logical {
public:
    Rel(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Rel(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

class Odd : public Logical {
public:
    Odd(const CodeToken& token, ExprNode expr, bool constant = false);
    Odd(CodeToken&& token, ExprNode expr, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;
};

using LogicalNode = shared_ptr<Logical>;
using OrNode = shared_ptr<Or>;
using AndNode = shared_ptr<And>;
using NotNode = shared_ptr<Not>;
using RelNode = shared_ptr<Rel>;
using OddNode = shared_ptr<Odd>;