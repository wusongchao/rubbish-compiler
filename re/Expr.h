#pragma once
#include "Ast.h"
#include "Type.h"
#include "CodeToken.h"

class Expr : public Ast {
public:
    Expr(const CodeToken& token, Type type);
    Expr(const CodeToken& token, Type type, bool constant);

    void accept(AstVisitor& visitor) override;

    bool isConstant()
    {
        return constant;
    }

    void setConstant(bool cond)
    {
        constant = cond;
    }
private:
    CodeToken token;
    Type type;
    bool constant = false;
};

class Id : public Expr {
public:
    Id(const CodeToken& token, Type type);
    Id(const CodeToken& token, Type type, bool constant);
};