#include "IRGenerator.h"

using std::stoi;

AstNode IRGenerator::visitConstant(ConstantNode expr)
{
    switch (expr->type.tag) {
        case TypeTag::Bool:
            if (expr->token.value == "true") {
                operateStack.push(make_shared<IR::Bool>(true));
            } else {
                operateStack.push(make_shared<IR::Bool>(false));
            }
            break;
        case TypeTag::Int:
            operateStack.push(make_shared<IR::Integer>(stoi(expr->token.value)));
    }

    return expr;
}

AstNode IRGenerator::visitId(IdNode id)
{
    operateStack.push(make_shared<IR::Id>(id));
    return id;
}

AstNode IRGenerator::visitProgram(shared_ptr<Program> program)
{
    return program;
}

AstNode IRGenerator::visitAssign(AssignNode assign)
{
    return AstNode();
}

AstNode IRGenerator::visitExpr(ExprNode expr)
{
    return visit(expr);
}

AstNode IRGenerator::visitArith(ArithNode arith)
{
    visit(arith->expr1);
    visit(arith->expr2);

    auto src1{ operateStack.top() };
    operateStack.pop();
    auto src2{ operateStack.top() };
    operateStack.pop();

    auto temp{ createTemp(src1->width) };
    switch (arith->token.tokenType) {
        case CodeTokenType::Add:
            emitArith(Opcode::Add, src1, src2, temp);
            break;
        case CodeTokenType::Sub:
            emitArith(Opcode::Sub, src1, src2, temp);
            break;
        case CodeTokenType::Mul:
            emitArith(Opcode::Mul, src1, src2, temp);
            break;
        case CodeTokenType::Div:
            emitArith(Opcode::Div, src1, src2, temp);
            break;
    }
    operateStack.push(temp);
    return arith;
}

AstNode IRGenerator::visitOp(OpNode op)
{
    return visit(op);
}

AstNode IRGenerator::visitUnary(UnaryNode unary)
{
    visit(unary->expr);
    auto src{ operateStack.top() };
    operateStack.pop();

    auto temp{ createTemp(src->width) };
    switch (unary->token.tokenType) {
        case CodeTokenType::Sub:
            emitUnary(Opcode::Add, src, temp);
            break;
    }
    operateStack.push(temp);
    return unary;
}

//AssignNode IRGenerator::visitAssign(AssignNode node)
//{
//    IdNode id = node->id;
//    ExprNode expr = node->expr;
//    visitExpr(expr);
//}
