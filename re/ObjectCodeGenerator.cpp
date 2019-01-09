#include "ObjectCodeGenerator.h"

using IR::VarTag;

void ObjectCodeGenerator::generate()
{
    if (quads.empty()) {
        return;
    }

    auto quad = quads[0];
    while (quad != nullptr) {
        translateIR(quad);
    }
}

void ObjectCodeGenerator::translateIR(QuadPtr quad)
{
    switch (quad->op) {
        case Opcode::Add:
        case Opcode::Sub:
        case Opcode::Mul:
        case Opcode::Div:
            translateBinary(quad);
            break;
        case Opcode::Neg:
            translateUnary(quad);
            break;
        case Opcode::Assign:
            translateAssign(quad);
            break;
        case Opcode::Read:
            translateRead(quad);
            break;
        case Opcode::Write:
            translateWrite(quad);
            break;
        case Opcode::Jlt:
        case Opcode::Jle:
        case Opcode::Jgt:
        case Opcode::Jge:
        case Opcode::Jeq:
        case Opcode::Jne:
            translateConditionJmp(quad);
            break;
        case Opcode::Jodd:
        case Opcode::Jnodd:
            translateOddJmp(quad);
            break;
        case Opcode::Jmp:
            translateJmp(quad);
            break;
        case Opcode::Param:
            translateParam(quad);
            break;
        case Opcode::Call:
            translateCall(quad);
            break;n
    }
}

void ObjectCodeGenerator::translateBinary(QuadPtr binary)
{
    processOperand(binary->src1);
    processOperand(binary->src2);
    emitBinaryOp(binary->op);
    processDest(binary->result);
}

void ObjectCodeGenerator::translateUnary(QuadPtr unary)
{
    processOperand(unary->src1);
    emitBinaryOp(unary->op);
    processDest(unary->result);
}

void ObjectCodeGenerator::translateConditionJmp(QuadPtr jmp)
{
    processOperand(jmp->src1);
    processOperand(jmp->src2);
    auto labelVar = static_pointer_cast<IR::Integer>(jmp->result);
    auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, labelTarget);
}

void ObjectCodeGenerator::translateOddJmp(QuadPtr jmp)
{
    processOperand(jmp->src1);
    auto labelVar = static_pointer_cast<IR::Integer>(jmp->result);
    auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, labelTarget);
}

void ObjectCodeGenerator::translateJmp(QuadPtr jmp)
{
    auto labelVar = static_pointer_cast<IR::Integer>(jmp->result);
    auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, labelTarget);
}

void ObjectCodeGenerator::translateAssign(QuadPtr assign)
{
    processOperand(assign->src1);
    emitStore(assign->result);
}
