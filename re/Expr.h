#pragma once
#include "Ast.h"
#include "Type.h"
#include "CodeToken.h"

class Expr : public Ast {
public:
    Expr(const CodeToken& token, const Type& type, bool constant = false);

    Expr(CodeToken&& token, const Type& type, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;

    bool isConstant()
    {
        return constant;
    }

    void setConstant(bool cond)
    {
        constant = cond;
    }

    virtual string toString() override;

    Type type;
    CodeToken token;
private:
    bool constant = false;
};

using ExprNode = shared_ptr<Expr>;

class Id : public Expr {
public:
    Id(const CodeToken& token, const Type& type, int offset, bool constant = false);

    Id(CodeToken&& token, const Type& type, int offset, bool constant = false);

    AstNode accept(AstVisitor& visitor) override;

    int offset;
};

class Constant : public Expr {
public:
    Constant(const CodeToken& token, const Type& type);

    Constant(CodeToken&& token, const Type& type);

    shared_ptr<Constant> static createInteger(int i);

    shared_ptr<Constant> static createBool(bool b);

    AstNode accept(AstVisitor& visitor) override;
};

class Temp : public Expr {
public:
    Temp(int number, const Type& type, bool constant = false);
};

using IdNode = shared_ptr<Id>;
using ConstantNode = shared_ptr<Constant>;
using TempNode = shared_ptr<Temp>;