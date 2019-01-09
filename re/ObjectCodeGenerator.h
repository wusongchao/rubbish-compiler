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
        shared_ptr<Env> topEnv,
        ostream& outputStream);
private:
    void emitLoad(VarNode id)
    {
        outputStream << "LOD" << ' '
            << topEnv->getSymbolDepth(id->toString())
            << ' , '
            << id->offset
            << endl;
    }

    void emitLit(VarNode constant)
    {
        outputStream << 'LIT' << ' '
            << '0'
            << ' , '
            << constant->toString()
            << endl;
    }

    void emitStore(VarNode id)
    {
        outputStream << "STO" << ' '
            << topEnv->getSymbolDepth(id->toString())
            << ' , '
            << id->offset
            << endl;
    }

    void emitBinaryOp(Opcode op)
    {
        outputStream << "OPR" << ' '
            << '0'
            << ' , '
            << opcodeToString(op)
            << endl;
    }

    void emitJmp(Opcode op, int target)
    {
        outputStream << toUpper(opcodeToString(op)) << ' '
            << '0'
            << ' , '
            << target
            << endl;
    }

    void processOperand(VarNode operand) {
        switch (operand->tag) {
            case VarTag::Id:
                emitLoad(operand);
                break;
            case VarTag::Integer:
                emitLit(operand);
                break;
        }
    }

    void processDest(VarNode dest) {
        switch (dest->tag) {
            case VarTag::Id:
                emitStore(dest);
        }
    }

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

    vector<QuadPtr> quads;
    vector<QuadPtr> labels;
    shared_ptr<Env> topEnv;

    ostream& outputStream;
};