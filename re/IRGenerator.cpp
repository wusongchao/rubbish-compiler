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
        {
            // B -> true  B.code = gen("goto" B.true)
            // B -> false  B.code = gen("goto" B.false) 
            int falseLabel = labelStack.top();
            labelStack.pop();
            int trueLabel = labelStack.top();
            labelStack.pop();

            if (expr->token.value == "true") {
                if (trueLabel != Fall) {
                    emitJmp(trueLabel);
                }
                
                //operateStack.push(make_shared<IR::Bool>(true));
            } else {
                if (falseLabel != Fall) {
                    emitJmp(falseLabel);
                }
                //operateStack.push(make_shared<IR::Bool>(false));
            }
            break;
        }
        case TypeTag::Int:
        {
            operateStack.push(make_shared<IR::Integer>(stoi(expr->token.value)));
            break;
        }
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
    return visitBody(program->block->body);
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
    int beginLabel = emitLabel();
    int trueLabel = Fall;
    int endLabel = emitLabel();
    
    // label(B.true)
    markLabel(beginLabel);
    // B.true = fall
    // B.false = S.next(endLabel)
    labelStack.push(trueLabel);
    labelStack.push(endLabel);
    // B.code
    visit(whileNode->cond);

    // S.code
    visit(whileNode->stmt);

    // gen(goto begin)
    emitJmp(beginLabel);

    markLabel(endLabel);

    return whileNode;
}

AstNode IRGenerator::visitCall(CallNode call)
{
    // call param1, param2, ... , paramn
    // generate code:
    // paramn
    // ...
    // param1
    // call
    const auto& params = call->param;
    for (auto it = params.rbegin(); it != params.rend(); ++it)
    {
        visit(*it);

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
    // for read i1, i2, ..., in
    // generate code:
    // load in
    // ...
    // load i1
    const auto& datas = read->datas;
    for (auto it = datas.rbegin(); it != datas.rend(); ++it)
    {
        visit(*it);

        // get the argument by visit expressions(do the evaluation)
        auto arg{ operateStack.top() };
        operateStack.pop();

        emitRead(arg);
    }

    return read;
}

AstNode IRGenerator::visitWrite(WriteNode write)
{
    // for write expr1, expr2, expr3...
    // generate code:
    // write i1,
    // write i2,
    // ...

    // the difference comes from the object machine's stack structure
    for (const auto& exp : write->datas) {
        visit(exp);

        auto arg{ operateStack.top() };
        operateStack.pop();

        emitWrite(arg);
    }

    return write;
}

AstNode IRGenerator::visitLogical(LogicalNode logical)
{
    return visit(logical);
}

AstNode IRGenerator::visitOr(OrNode orNode)
{
    // also see the "dragon book" p261
    // for more information about the sequence, 
    // see visitRel's comment
    // inherited attributes B.true, B.false
    int parentFalse = labelStack.top();
    labelStack.pop();
    int parentTrue = labelStack.top();
    labelStack.pop();

    // if fall
    // pass the expr2
    int expr1True = parentTrue == Fall ? emitLabel() : parentTrue;
    int expr1False = Fall;
    int expr2True = parentTrue;
    int expr2False = parentFalse;
    
    // B1.code || B2.code
    labelStack.push(expr1True);
    labelStack.push(expr1False);
    visit(orNode->expr1);
    labelStack.push(expr2True);
    labelStack.push(expr2False);
    visit(orNode->expr2);
    // if B.true == fall
    if (parentTrue == Fall) {
        markLabel(expr1True);
    }

    return orNode;
}

AstNode IRGenerator::visitAnd(AndNode andNode)
{
    // also see the "dragon book" p261
    // for more information about the sequence, 
    // see visitRel's comment
    // inherited attributes B.true, B.false
    int parentFalse = labelStack.top();
    labelStack.pop();
    int parentTrue = labelStack.top();
    labelStack.pop();

    // B -> B1 && B2
    // B1.true = fall(must visit b2 cond)
    // B1.false must pass B2.code
    // B1.false = B.false == Fall? emitLabel() : B.false
    // B2.true = B1.true
    // B2.false = B.false
    int expr1True = Fall;
    int expr1False = parentFalse == Fall? emitLabel() : parentFalse;
    int expr2True = parentTrue;
    int expr2False = parentFalse;

    // B1.code || B2.code
    labelStack.push(expr1True);
    labelStack.push(expr1False);
    visit(andNode->expr1);
    labelStack.push(expr2True);
    labelStack.push(expr2False);
    visit(andNode->expr2);
    // if B.false == fall
    if (parentFalse == Fall) {
        markLabel(expr1False);
    }

    return andNode;
}

AstNode IRGenerator::visitNot(NotNode notNode)
{
    int falseLabel = labelStack.top();
    labelStack.pop();
    int trueLabel = labelStack.top();
    labelStack.pop();

    // B1.true = B.false
    // B1.false = B.true
    labelStack.push(falseLabel);
    labelStack.push(trueLabel);
    visit(notNode->expr1);

    return notNode;
}

AstNode IRGenerator::visitOdd(OddNode oddNode)
{
    // B -> odd E
    // similar rule as rel
    visit(oddNode->expr1);

    auto src{ operateStack.top() };
    operateStack.pop();

    int falseLabel = labelStack.top();
    labelStack.pop();
    int trueLabel = labelStack.top();
    labelStack.pop();

    CodeTokenType opTokenType = oddNode->token.tokenType;
    Opcode jmpRelOp = getJmpOpcode(opTokenType);

    if (trueLabel != Fall && falseLabel != Fall) {
        emitOddJmp(jmpRelOp, src, trueLabel);
        emitJmp(falseLabel);
    } else if (trueLabel != Fall) {
        emitOddJmp(jmpRelOp, src, trueLabel);
    } else if (falseLabel != Fall) {
        emitOddJmp(getOppositeConditonJmpOpcode(jmpRelOp), src, falseLabel);
    }

    return oddNode;
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
    Opcode jmpRelOp = getJmpOpcode(opTokenType);

    if (trueLabel != Fall && falseLabel != Fall) {
        emitConditionJmp(jmpRelOp, src1, src2, trueLabel);
        emitJmp(falseLabel);
    } else if (trueLabel != Fall) {
        emitConditionJmp(jmpRelOp, src1, src2, trueLabel);
    } else if (falseLabel != Fall) {
        emitConditionJmp(getOppositeConditonJmpOpcode(jmpRelOp), src1, src2, falseLabel);
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
            emitUnary(Opcode::Neg, src, temp);
            break;
    }
    operateStack.push(temp);
    return unary;
}