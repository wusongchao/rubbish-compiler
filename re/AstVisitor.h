#pragma once
#include "Ast.h"
#include "Expr.h"

class AstVisitor {
public:
    virtual AstNode visit(AstNode astNode);

    virtual shared_ptr<Program> visitProgram(shared_ptr<Program> program) = 0;

    virtual ExprNode visitExpr(ExprNode expr) = 0;
};