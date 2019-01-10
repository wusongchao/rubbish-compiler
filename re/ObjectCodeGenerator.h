#pragma once
#include "Env.h"
#include "IRGenerator.h"
#include "ObjectCode.h"
#include "UtilMethods.h"

using std::endl;
using IR::opcodeToString;
using IR::VarTag;
using namespace ObjectCode;
using CodePtr = shared_ptr<Code>;
using TargetPtr = shared_ptr<Target>;
using std::pair;

class ObjectCodeGenerator {
public:
    void generate();

    void output(ostream& stream) {
        for (const auto& code : codes) {
            stream << code->toString() << endl;
        }
    }

    ObjectCodeGenerator(const vector<QuadPtr>& quads,
        BlockNode topBlock);
private:
    void emitLoad(VarNode id)
    {
        int depth = topBlock->top->getSymbolDepth(id->toString());
        codes.push_back(make_shared<Load>(depth, id->offset));
    }

    void emitLit(VarNode constant)
    {
        //outputStream << "LIT" << ' '
        //    << '0'
        //    << " , "
        //    << constant->toString()
        //    << endl;
        codes.push_back(make_shared<Lit>(constant->toString()));
    }

    void emitStore(VarNode id)
    {
        //outputStream << "STO" << ' '
        //    << topBlock->top->getSymbolDepth(id->toString())
        //    << " , "
        //    << id->offset
        //    << endl;
        int depth = topBlock->top->getSymbolDepth(id->toString());
        codes.push_back(make_shared<Store>(depth, id->offset));
    }

    void emitBinaryOp(Opcode op)
    {
        //outputStream << "OPR" << ' '
        //    << '0'
        //    << " , "
        //    << opcodeToString(op)
        //    << endl;
        codes.push_back(make_shared<BinaryOp>(opcodeToString(op)));
    }

    void emitRead(VarNode id)
    {
        int depth = topBlock->top->getSymbolDepth(id->toString());
        codes.push_back(make_shared<ObjectCode::Read>(depth, id->offset));
    }

    void emitWrite()
    {
        codes.push_back(make_shared<ObjectCode::Write>());
    }

    void emitInt(int value)
    {
        codes.push_back(make_shared<Int>(value));
    }

    // when meet a reference to label
    void recordLabel(int labelNumber) {
        beReferencedLabels[labelNumber].second.push_back(jmpCodes.size());
    }

    // when meet a definition of label
    void markLabel(int labelNumber) {
        beReferencedLabels[labelNumber].first = codes.size();
    }

    // JPC and JMP
    void emitJmp(Opcode op, VarNode target)
    {
        //outputStream << toUpper(opcodeToString(op)) << ' '
        //    << '0'
        //    << " , "
        //    << labelNumber
        //    << endl;
        int labelNumber = static_pointer_cast<IR::Integer>(target)->value;
        auto jmp{ make_shared<Jmp>(labelNumber, toUpper(opcodeToString(op))) };
        codes.push_back(jmp);
        recordLabel(labelNumber);
        jmpCodes.push_back(jmp);
        // insert the object code
    }

    void emitCall(VarNode label, VarNode id)
    {
        int labelNumber = static_pointer_cast<IR::Integer>(label)->value;
        //outputStream << "CAL" << ' '
        //    << '0'
        //    << " , "
        //    << func->toString()
        //    << endl;
        auto call{ make_shared<ObjectCode::Call>(labelNumber) };
        codes.push_back(call);
        recordLabel(labelNumber);
        jmpCodes.push_back(call);
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
    unordered_map<int, pair<int, vector<int>>> beReferencedLabels;
    BlockNode topBlock;
    vector<CodePtr> codes;
    vector<TargetPtr> jmpCodes;
};