#pragma once
#include "Ast.h"
#include "Type.h"
#include "CodeToken.h"

class Expr : public Ast {
public:
    Expr(const CodeToken& token, const Type& type, bool constant = false);

    Expr(CodeToken&& token, const Type& type, bool constant = false);

    void accept(AstVisitor& visitor) override;

    bool isConstant()
    {
        return constant;
    }

    void setConstant(bool cond)
    {
        constant = cond;
    }

    Type type;
    CodeToken token;
private:
    bool constant = false;
};

using ExprNode = shared_ptr<Expr>;

class Id : public Expr {
public:
    Id(const CodeToken& token, const Type& type, bool constant = false);

    Id(CodeToken&& token, const Type& type, bool constant = false);
};

class Constant : public Expr {
public:
    Constant(const CodeToken& token, const Type& type);

    Constant(CodeToken&& token, const Type& type);

    shared_ptr<Constant> static createInteger(int i);

    shared_ptr<Constant> static createBool(bool b);
};

