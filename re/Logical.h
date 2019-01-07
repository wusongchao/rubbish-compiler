#pragma once
#include "Expr.h"

class Logical : public Expr{
public:
    Logical(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Logical(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);

    ExprNode expr1, expr2;
};

class Or : public Logical {
public:
    Or(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Or(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);
};

class And : public Logical {
public:
    And(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    And(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);
};

class Not : public Logical {
public:
    Not(const CodeToken& token, ExprNode expr, bool constant = false);
    Not(CodeToken&& token, ExprNode expr, bool constant = false);
};

class Rel : public Logical {
public:
    Rel(const CodeToken& token, ExprNode p1, ExprNode p2, bool constant = false);
    Rel(CodeToken&& token, ExprNode p1, ExprNode p2, bool constant = false);
};