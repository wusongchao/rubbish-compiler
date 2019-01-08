#pragma once
#include "Ast.h"
#include "Expr.h"
#include "Op.h"

class AstVisitor {
public:
    virtual AstNode visit(AstNode astNode);

    virtual AstNode visitProgram(shared_ptr<Program> program) = 0;

    virtual AstNode visitExpr(ExprNode expr) = 0;

    virtual AstNode visitArith(ArithNode arith) = 0;

    virtual AstNode visitOp(OpNode op) = 0;

    virtual AstNode visitUnary(UnaryNode unary) = 0;

    virtual AstNode visitConstant(ConstantNode constant) = 0;

    virtual AstNode visitId(IdNode id) = 0;
};