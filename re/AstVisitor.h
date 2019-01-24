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

using namespace AST;

class AstVisitor {
public:
    virtual Ast* visit(Ast* ast);

    virtual Ast* visitProgram(Program* program) = 0;

    virtual Ast* visitBlock(Block* block) = 0;

    virtual Ast* visitProcedure(Proc* proc) = 0;

    virtual Ast* visitBody(Body* body) = 0;

    virtual Ast* visitAssign(Assign* assign) = 0;

    virtual Ast* visitIf(If* ifNode) = 0;

    virtual Ast* visitWhile(While* whileNode) = 0;

    virtual Ast* visitCall(Call* call) = 0;

    virtual Ast* visitRead(Read* read) = 0;

    virtual Ast* visitWrite(Write* write) = 0;

    virtual Ast* visitExpr(Expr* expr) = 0;

    virtual Ast* visitLogical(Logical* logical) = 0;

    virtual Ast* visitRel(Rel* rel) = 0;

    virtual Ast* visitOr(Or* orNode) = 0;

    virtual Ast* visitAnd(And* andNode) = 0;

    virtual Ast* visitNot(Not* notNode) = 0;

    virtual Ast* visitOdd(Odd* oddNode) = 0;

    virtual Ast* visitArith(Arith* arith) = 0;

    virtual Ast* visitOp(Op* op) = 0;

    virtual Ast* visitUnary(Unary* unary) = 0;

    virtual Ast* visitConstant(Constant* constant) = 0;

    virtual Ast* visitId(Id* id) = 0;
};