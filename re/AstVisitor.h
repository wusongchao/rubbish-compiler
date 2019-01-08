#pragma once
#include "Ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "Op.h"
#include "Logical.h"

class AstVisitor {
public:
    virtual AstNode visit(AstNode astNode);

    virtual AstNode visitProgram(ProgramNode program) = 0;

    virtual AstNode visitBody(BodyNode body) = 0;

    virtual AstNode visitAssign(AssignNode assign) = 0;

    virtual AstNode visitIf(IfNode ifNode) = 0;

    virtual AstNode visitWhile(WhileNode whileNode) = 0;

    virtual AstNode visitCall(CallNode call) = 0;

    virtual AstNode visitRead(ReadNode read) = 0;

    virtual AstNode visitWrite(WriteNode write) = 0;

    virtual AstNode visitExpr(ExprNode expr) = 0;

    virtual AstNode visitLogical(LogicalNode logical) = 0;

    virtual AstNode visitRel(RelNode rel) = 0;

    virtual AstNode visitOr(OrNode orNode) = 0;

    virtual AstNode visitAnd(AndNode andNode) = 0;

    virtual AstNode visitNot(NotNode notNode) = 0;

    virtual AstNode visitOdd(OddNode oddNode) = 0;

    virtual AstNode visitArith(ArithNode arith) = 0;

    virtual AstNode visitOp(OpNode op) = 0;

    virtual AstNode visitUnary(UnaryNode unary) = 0;

    virtual AstNode visitConstant(ConstantNode constant) = 0;

    virtual AstNode visitId(IdNode id) = 0;
};