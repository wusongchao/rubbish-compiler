#include "IRGenerator.h"

using std::stoi;


// operatorStack.push, similar to return value, synthesized attributes
// operatorStack.pop, similar to parameter, inherited attributes
// when expand a sub procedure
// it accept its parameter from stack
// and return value by pushing to the stack
AstNode IRGenerator::visitConstant(ConstantNode expr)
{
    switch (expr->type.tag) {
        case TypeTag::Bool:
            // B -> true  B.code = gen("goto" B.true)
            // B -> false  B.code = gen("goto" B.false) 
            if (expr->token.value == "true") {
                int trueLabel = labelStack.top();
                labelStack.pop();
                emitJmp(trueLabel);
                //operateStack.push(make_shared<IR::Bool>(true));
            } else {
                int falseLabel = labelStack.top();
                labelStack.pop();
                emitJmp(falseLabel);
                //operateStack.push(make_shared<IR::Bool>(false));
            }
            break;
        case TypeTag::Int:
            operateStack.push(make_shared<IR::Integer>(stoi(expr->token.value)));
            break;
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
        int trueLabel = Fall;
        int elseLabel = emitLabel();
        int endLabel = emitLabel();

        // B.true = fall
        // B.false = newLabel()
        labelStack.push(trueLabel);
        labelStack.push(elseLabel);
        // generate B.code
        visit(ifNode->cond);
       
        // generate S1.code
        visit(ifNode->trueStmt);

        // goto S.next(endLabel)
        emitJmp(endLabel);

        // label(B.false) elseLabel
        markLabel(elseLabel);
        // generate S2.code
        visit(ifNode->falseStmt);
        markLabel(endLabel);
    } else {
        int trueLabel = Fall;
        int endLabel = emitLabel();

        // B.true = fall
        // B.false = S.next(endLabel)
        labelStack.push(trueLabel);
        labelStack.push(endLabel);
        // generate B.code
        visit(ifNode->cond);

        // generate S1.code
        // see the "dragon book" p261
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

AstNode IRGenerator::visitLogical(LogicalNode logical)
{
    return visit(logical);
}

AstNode IRGenerator::visitOr(OrNode orNode)
{
    return AstNode();
}

AstNode IRGenerator::visitAnd(AndNode andNode)
{
    return AstNode();
}

AstNode IRGenerator::visitNot(NotNode notNode)
{
    return AstNode();
}

AstNode IRGenerator::visitOdd(OddNode oddNode)
{
    return AstNode();
}

AstNode IRGenerator::visitRel(RelNode rel)
{
    visit(rel->expr1);
    visit(rel->expr2);

    // src1 was pushed into the stack first
    auto src2{ operateStack.top() };
    operateStack.pop();
    auto src1{ operateStack.top() };
    operateStack.pop();

    // the same rule as below
    // since B.true was pushed into the stack first
    int falseLabel = labelStack.top();
    labelStack.pop();
    int trueLabel = labelStack.top();
    labelStack.pop();

    //if (B.true != fall and B.false != fall){
    //    gen(if test goto B.true) || gen(goto B.false)
    //} else if (B.true != fall) {
    //    gen(if test goto B.true)
    //} else if (B.false != fall) {
    //    gen(iffalse test goto B.false)
    //}

    CodeTokenType opTokenType = rel->token.tokenType;
    Opcode relOp = getOpcode(opTokenType);

    if (trueLabel != Fall && falseLabel != Fall) {
        emitConditionJmp(relOp, src1, src2, trueLabel);
        emitJmp(falseLabel);
    } else if (trueLabel != Fall) {
        emitConditionJmp(relOp, src1, src2, trueLabel);
    } else if (falseLabel != Fall) {
        emitConditionJmp(getOppositeConditonJmpOpcode(relOp), src1, src2, falseLabel);
    }

    return rel;
}

AstNode IRGenerator::visitArith(ArithNode arith)
{
    visit(arith->expr1);
    visit(arith->expr2);

    auto src2{ operateStack.top() };
    operateStack.pop();
    auto src1{ operateStack.top() };
    operateStack.pop();

    // similar to visitArith(visit(expr1), visit(expr2))

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