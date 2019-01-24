#include "IRGenerator.h"

using std::stoi;


// operatorStack.push, similar to return value, synthesized attributes
// operatorStack.pop, similar to parameter, inherited attributes
// when expand a sub procedure
// it accept its parameter from stack
// and return value by pushing to the stack
Ast* IRGenerator::visitConstant(Constant* expr)
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

Ast* IRGenerator::visitId(Id* id)
{
    operateStack.push(make_shared<IR::Id>(id));
    return id;
}

Ast* IRGenerator::visitProgram(Program* program)
{
    visitBlock(program->block.get());
    return program;
}

Ast* IRGenerator::visitBody(Body* body)
{
    for (const auto& stmt : body->stmts) {
        visit(stmt.get());
    }
    return body;
}

Ast* IRGenerator::visitBlock(Block* block)
{
    // jump the procedure definition code
    if (!block->procs.empty()) {
        int bodyLabel = emitLabel();
        emitJmp(bodyLabel);
        for (const auto& proc : block->procs) {
            visit(proc.get());
        }
        markLabel(bodyLabel);
    }
    visit(block->body.get());
    return block;
}

Ast* IRGenerator::visitProcedure(Proc* proc)
{
    int procLabel = emitLabel(proc->id);
    markLabel(procLabel);
    visit(proc->block.get());
    return proc;
}

Ast* IRGenerator::visitAssign(Assign* assign)
{
    visit(assign->expr.get());
    auto src{ operateStack.top() };
    operateStack.pop();

    auto dest{ make_shared<IR::Id>(assign->id) };
    emitAssign(std::move(src), std::move(dest));

    return assign;
}

Ast* IRGenerator::visitExpr(Expr* expr)
{
    return visit(expr);
}

Ast* IRGenerator::visitIf(If* ifNode)
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
        visit(ifNode->cond.get());
       
        // generate S1.code
        visit(ifNode->trueStmt.get());

        // goto S.next(endLabel)
        emitJmp(endLabel);

        // label(B.false) elseLabel
        markLabel(elseLabel);
        // generate S2.code
        visit(ifNode->falseStmt.get());
        markLabel(endLabel);
    } else {
        int trueLabel = Fall;
        int endLabel = emitLabel();

        // B.true = fall
        // B.false = S.next(endLabel)
        labelStack.push(trueLabel);
        labelStack.push(endLabel);
        // generate B.code
        visit(ifNode->cond.get());

        // generate S1.code
        // see the "dragon book" p261
        visit(ifNode->trueStmt.get());

        markLabel(endLabel);
    }
    
    return ifNode;
}

Ast* IRGenerator::visitWhile(While* whileNode)
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
    visit(whileNode->cond.get());

    // S.code
    visit(whileNode->stmt.get());

    // gen(goto begin)
    emitJmp(beginLabel);

    markLabel(endLabel);

    return whileNode;
}

Ast* IRGenerator::visitCall(Call* call)
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
        visit(it->get());

        // get the argument by visit expressions(do the evaluation)
        auto arg{ operateStack.top() };
        operateStack.pop();

        emitParam(arg);
    }

    int i = 0;
    for (int size = labels.size();i < size;i++) {
        const auto& label = labels[i];
        if (label->src2 != nullptr && label->src2->toString() == call->id->toString()) {
            break;
        }
    }

    emitCall(make_shared<IR::Integer>(i), make_shared<IR::Id>(call->id));
    return call;
}

Ast* IRGenerator::visitRead(Read* read)
{
    // for read i1, i2, ..., in
    // generate code:
    // load in
    // ...
    // load i1
    const auto& datas = read->datas;
    for (auto it = datas.rbegin(); it != datas.rend(); ++it)
    {
        visit(it->get());

        // get the argument by visit expressions(do the evaluation)
        auto arg{ operateStack.top() };
        operateStack.pop();

        emitRead(arg);
    }

    return read;
}

Ast* IRGenerator::visitWrite(Write* write)
{
    // for write expr1, expr2, expr3...
    // generate code:
    // write i1,
    // write i2,
    // ...

    // the difference comes from the object machine's stack structure
    for (const auto& exp : write->datas) {
        visit(exp.get());

        auto arg{ operateStack.top() };
        operateStack.pop();

        emitWrite(arg);
    }

    return write;
}

Ast* IRGenerator::visitLogical(Logical* logical)
{
    return visit(logical);
}

Ast* IRGenerator::visitOr(Or* orNode)
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
    visit(orNode->expr1.get());
    labelStack.push(expr2True);
    labelStack.push(expr2False);
    visit(orNode->expr2.get());
    // if B.true == fall
    if (parentTrue == Fall) {
        markLabel(expr1True);
    }

    return orNode;
}

Ast* IRGenerator::visitAnd(And* andNode)
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
    visit(andNode->expr1.get());
    labelStack.push(expr2True);
    labelStack.push(expr2False);
    visit(andNode->expr2.get());
    // if B.false == fall
    if (parentFalse == Fall) {
        markLabel(expr1False);
    }

    return andNode;
}

Ast* IRGenerator::visitNot(Not* notNode)
{
    int falseLabel = labelStack.top();
    labelStack.pop();
    int trueLabel = labelStack.top();
    labelStack.pop();

    // B1.true = B.false
    // B1.false = B.true
    labelStack.push(falseLabel);
    labelStack.push(trueLabel);
    visit(notNode->expr1.get());

    return notNode;
}

Ast* IRGenerator::visitOdd(Odd* oddNode)
{
    // B -> odd E
    // similar rule as rel
    visit(oddNode->expr1.get());

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

Ast* IRGenerator::visitRel(Rel* rel)
{
    visit(rel->expr1.get());
    auto src1{ operateStack.top() };
    operateStack.pop();

    visit(rel->expr2.get());
    auto src2{ operateStack.top() };
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

Ast* IRGenerator::visitArith(Arith* arith)
{
    visit(arith->expr1.get());
    auto src1{ operateStack.top() };
    operateStack.pop();

    visit(arith->expr2.get());
    auto src2{ operateStack.top() };
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

Ast* IRGenerator::visitOp(Op* op)
{
    return visit(op);
}

Ast* IRGenerator::visitUnary(Unary* unary)
{
    visit(unary->expr.get());
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