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
        quad = quad->next;
    }
    backpatchJmps();
}

ObjectCodeGenerator::ObjectCodeGenerator(const vector<QuadPtr>& quads, BlockNode topBlock)
    :quads(quads), topBlock(topBlock)
{
}

void ObjectCodeGenerator::backpatchJmps()
{
    for (auto& item : beReferencedLabels) {
        int actualTarget = item.second.first;
        const auto& jmpIndex = item.second.second;
        for (int index : jmpIndex) {
            jmpCodes[index]->target = actualTarget;
        }
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
            break;
        case Opcode::Label:
            translateLabel(quad);
            break;
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

void ObjectCodeGenerator::translateRead(QuadPtr read)
{
    emitRead(read->src1);
}

void ObjectCodeGenerator::translateWrite(QuadPtr write)
{
    emitWrite();
}

void ObjectCodeGenerator::translateCall(QuadPtr call)
{
    emitCall(call->src1, call->result);
}

void ObjectCodeGenerator::translateParam(QuadPtr param)
{
    // push into stack
    processOperand(param->src1);
}

void ObjectCodeGenerator::translateConditionJmp(QuadPtr jmp)
{
    processOperand(jmp->src1);
    processOperand(jmp->src2);
    //int labelNumber = static_pointer_cast<IR::Integer>(jmp->result)->value;
    //auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, jmp->result);
}

void ObjectCodeGenerator::translateOddJmp(QuadPtr jmp)
{
    processOperand(jmp->src1);
    //int labelNumber = static_pointer_cast<IR::Integer>(jmp->result)->value;
    //auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, jmp->result);
}

void ObjectCodeGenerator::translateJmp(QuadPtr jmp)
{
    //int labelNumber = static_pointer_cast<IR::Integer>(jmp->result)->value;
    //auto labelTarget = stoi(labels[labelVar->value]->result->toString());
    emitJmp(jmp->op, jmp->result);
}

void ObjectCodeGenerator::translateAssign(QuadPtr assign)
{
    processOperand(assign->src1);
    emitStore(assign->result);
}

void ObjectCodeGenerator::translateLabel(QuadPtr label)
{
    int labelNumber = static_pointer_cast<IR::Integer>(label->src1)->value;
    
    // if not exist, insert
    // it might existed before the definition of label
    // for example:
    // jmp l1
    // ...
    // l1:
    // ...
    beReferencedLabels[labelNumber].first = codes.size();

    // when meet the definition, can define its actual position
}
