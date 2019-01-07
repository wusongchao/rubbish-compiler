#pragma once
#include "Ast.h"
#include "Expr.h"

class AstVisitor {
public:
    virtual void visit(shared_ptr<Ast> astNode);

    virtual void visitProgram(shared_ptr<Program> program) = 0;

    virtual void visitExpr(shared_ptr<Expr> expr) = 0;
};