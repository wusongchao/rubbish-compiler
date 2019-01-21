#pragma once
#include "Ast.h"
#include "Expr.h"
#include "Stmt.h"
#include "Op.h"
#include "Logical.h"

// take raw pointer as input
//Guideline: Don¡¯t pass a smart pointer as a function parameter unless 
//you want to use or manipulate the smart pointer itself, 
//such as to share or transfer ownership.
//
//Guideline : Express that a function will store and share ownership of a heap object 
//using a by - value shared_ptr parameter.
//
//Guideline : Use a non - const shared_ptr& parameter only to modify the shared_ptr.
//Use a const shared_ptr& as a parameter only if you¡¯re not sure whether or not you¡¯ll take a copy and share ownership; 
//otherwise use widget* instead(or if not nullable, a widget&).

class AstVisitor {
public:
    virtual AstNode visit(AstNode astNode);

    virtual AstNode visitProgram(ProgramNode program) = 0;

    virtual AstNode visitBlock(BlockNode block) = 0;

    virtual AstNode visitProcedure(ProcNode proc) = 0;

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