#pragma once
#include "Env.h"
#include "IRGenerator.h"
#include "UtilMethods.h"

using std::endl;
using IR::opcodeToString;
using IR::VarTag;

class ObjectCodeGenerator {
public:
    void generate();

    ObjectCodeGenerator(const vector<QuadPtr>& quads,
        const vector<QuadPtr>& labels,
        BlockNode topBlock,
        ostream& outputStream);
private:
    void emitLoad(VarNode id)
    {
        auto s = id->toString();
        outputStream << "LOD" << ' '
            << topBlock->top->getSymbolDepth(id->toString())
            << " , "
            << id->offset
            << endl;
    }

    void emitLit(VarNode constant)
    {
        outputStream << "LIT" << ' '
            << '0'
            << " , "
            << constant->toString()
            << endl;
    }

    void emitStore(VarNode id)
    {
        outputStream << "STO" << ' '
            << topBlock->top->getSymbolDepth(id->toString())
            << " , "
            << id->offset
            << endl;
    }

    void emitBinaryOp(Opcode op)
    {
        outputStream << "OPR" << ' '
            << '0'
            << " , "
            << opcodeToString(op)
            << endl;
    }

    void emitRead(VarNode id)
    {
        outputStream << "RED" << ' '
            << topBlock->top->getSymbolDepth(id->toString())
            << " , "
            << id->offset
            << endl;
    }

    void emitWrite()
    {
        outputStream << "WRT 0 , 0" << endl;
    }

    void emitInt(int value)
    {
        outputStream << "INT 0, "
            << value
            << endl;
    }

    // JPC and JMP
    void emitJmp(Opcode op, int target)
    {
        outputStream << toUpper(opcodeToString(op)) << ' '
            << '0'
            << " , "
            << target
            << endl;
    }

    void emitCall(VarNode func)
    {
        outputStream << "CAL" << ' '
            << '0'
            << " , "
            << func->toString()
            << endl;
    }

    // push the operand into the stack
    void processOperand(VarNode operand) {
        switch (operand->tag) {
            case VarTag::Id:
                ++offset;
                emitLoad(operand);
                break;
            case VarTag::Integer:
                ++offset;
                emitLit(operand);
                break;
        }
    }

    void processDest(VarNode dest) {
        switch (dest->tag) {
            case VarTag::Id:
                ++offset;
                emitStore(dest);
        }
    }

    void 

    void translateIR(QuadPtr quad);

    void translateBinary(QuadPtr binary);

    void translateUnary(QuadPtr unary);

    void translateRead(QuadPtr read);

    void translateWrite(QuadPtr write);

    void translateCall(QuadPtr call);

    void translateParam(QuadPtr param);

    void translateConditionJmp(QuadPtr jmp);

    void translateOddJmp(QuadPtr jmp);

    void translateJmp(QuadPtr jmp);

    void translateAssign(QuadPtr assign);

    void translateLabel(QuadPtr label);

    vector<QuadPtr> quads;
    vector<QuadPtr> labels;
    BlockNode topBlock;

    ostream& outputStream;

    int offset = 0;
};