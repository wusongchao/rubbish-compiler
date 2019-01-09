#pragma once
#include "Env.h"
#include "IRGenerator.h"
#include "ObjectCode.h"
#include "UtilMethods.h"

using std::endl;
using IR::opcodeToString;
using IR::VarTag;
using CodePtr = shared_ptr<ObjectCode::ObjectCode>;
using std::pair;

class ObjectCodeGenerator {
public:
    void generate();

    void output(ostream& stream) {
        for (const auto& code : objectCodes) {
            stream << code->toString() << endl;
        }
    }

    ObjectCodeGenerator(const vector<QuadPtr>& quads,
        BlockNode topBlock);
private:
    void emitLoad(VarNode id)
    {
        auto s = id->toString();
        outputStream << "LOD" << ' '
            << topBlock->top->getSymbolDepth(id->toString())
            << " , "
            << id->offset
            << endl;
        objectCodes.push_back(make_shared<>());
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
    void emitJmp(Opcode op, int labelNumber)
    {
        outputStream << toUpper(opcodeToString(op)) << ' '
            << '0'
            << " , "
            << labelNumber
            << endl;
        beReferencedLabels[labelNumber].second.push_back(jmpCodes.size());
        // insert the object code
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

    void backpatchJmps();

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
    //vector<QuadPtr> labels;
    unordered_map<int, pair<int, vector<int>>> beReferencedLabels;
    BlockNode topBlock;
    vector<CodePtr> objectCodes;
    vector<CodePtr> jmpCodes;
};