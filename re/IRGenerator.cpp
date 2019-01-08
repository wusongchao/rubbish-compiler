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
    return visitBody(program->body);
}

AstNode IRGenerator::visitBody(BodyNode body)
{
    for (const auto& stmt : body->stmts) {
        visit(stmt);
    }
    return body;
}

AstNode IRGenerator::visitAssign(AssignNode assign)
{
    visit(assign->expr);
    auto src{ operateStack.top() };
    operateStack.pop();

    auto dest{ make_shared<IR::Id>(assign->id) };
    emitAssign(src, dest);

    return assign;
}

AstNode IRGenerator::visitExpr(ExprNode expr)
{
    return visit(expr);
}

AstNode IRGenerator::visitIf(IfNode ifNode)
{
    //int ifLabel = emitLabel();
    // logical expression
    if (ifNode->falseStmt != nullptr) {
        int elseLabel = emitLabel();
        int endLabel = emitLabel();

        visit(ifNode->cond);
        auto cond{ logicalStack.top() };
        logicalStack.pop();

        emitConditionJmp(getOppositeConditonJmp(cond->op), cond->src1, cond->src2, elseLabel);

        visit(ifNode->trueStmt);

        emitJmp(endLabel);

        markLabel(elseLabel);
        visit(ifNode->falseStmt);
        markLabel(endLabel);
    } else {
        int endLabel = emitLabel();

        visit(ifNode->cond);
        auto cond{ logicalStack.top() };
        logicalStack.pop();

        emitConditionJmp(getOppositeConditonJmp(cond->op), cond->src1, cond->src2, endLabel);

        visit(ifNode->trueStmt);

        markLabel(endLabel);
    }
    
    return ifNode;
}

AstNode IRGenerator::visitWhile(WhileNode whileNode)
{
    return AstNode();
}

AstNode IRGenerator::visitCall(CallNode call)
{
    for (const auto& param : call->param) {
        visit(param);

        // get the argument by visit expressions(do the evaluation)
        auto arg{ operateStack.top() };
        operateStack.pop();
        emitParam(arg);
    }

    auto func{ make_shared<IR::Id>(call->id) };
    emitCall(func);
    return call;
}

AstNode IRGenerator::visitRead(ReadNode read)
{
    return AstNode();
}

AstNode IRGenerator::visitWrite(WriteNode write)
{
    return AstNode();
}

AstNode IRGenerator::visitRel(RelNode rel)
{
    visit(rel->expr1);
    visit(rel->expr2);

    auto src1{ operateStack.top() };
    operateStack.pop();
    auto src2{ operateStack.top() };
    operateStack.pop();

    switch (rel->token.tokenType) {
        case CodeTokenType::LT:
            emitLogical(Opcode::LT, src1, src2);
            break;
        case CodeTokenType::LE:
            emitLogical(Opcode::LE, src1, src2);
            break;
        case CodeTokenType::GT:
            emitLogical(Opcode::GT, src1, src2);
            break;
        case CodeTokenType::GE:
            emitLogical(Opcode::GE, src1, src2);
            break;
        case CodeTokenType::EQ:
            emitLogical(Opcode::EQ, src1, src2);
            break;
        case CodeTokenType::NE:
            emitLogical(Opcode::NE, src1, src2);
            break;
    }
    // will not create temp variable
    return rel;
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