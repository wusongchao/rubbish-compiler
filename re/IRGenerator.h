#pragma once
#include <vector>
#include <stack>

#include "AstVisitor.h"
#include "IR.h"

using std::vector;
using std::make_shared;
using std::stack;

using IR::Quad;
using IR::Var;
using IR::VarNode;
using IR::Opcode;

class IRGenerator : public AstVisitor{
public:

    const vector<Quad>& getQuads() const
    {
        return quads;
    }

    const vector<int> getLabels() const
    {
        return labels;
    }

    AstNode visitProgram(shared_ptr<Program> program) override;

    AstNode visitExpr(ExprNode expr) override;

    AstNode visitArith(ArithNode arith) override;

    AstNode visitOp(OpNode op) override;

    AstNode visitUnary(UnaryNode unary) override;

    AstNode visitConstant(ConstantNode constant) override;

    AstNode visitId(IdNode id) override;


private:
    void emitGoto(int label) {
        quads.emplace_back();
    }

    //TempNode emitBinary(const CodeToken& opToken, const Type& resType, const ExprNode& lhs, const ExprNode& rhs);

    shared_ptr<IR::Temp> createTemp(const Type& type) {
        return make_shared<IR::Temp>(tempIndex++, type);
    }

    shared_ptr<IR::Temp> createTemp(int width) {
        return make_shared<IR::Temp>(tempIndex++, width);
    }

    void emitConditionJmp() {

    }

    void emitArith(Opcode op, VarNode src1, VarNode src2, VarNode dest) {
        quads.emplace_back(op, src1, src2, dest);
    }

    void emitUnary(Opcode op, VarNode src, VarNode dest) {
        quads.emplace_back(op, src, dest);
    }

    int defineLabel() {
        labels.push_back(0);

        // L0 is a special label
        // label starts from L1
        return labels.size();
    }

    void markLabel(int label) {
        labels[label] = quads.size();
    }

    int tempIndex = 1;
    vector<Quad> quads;
    vector<int> labels;
    stack<shared_ptr<Var>> operateStack;
    // label number -> offset address
    // L1:L3:
    //   i = i + 1
    // L5: 
    //   t1 = i + 8
    //   t2 = a [ t1 ]
    //   if t2 < v goto L3
    // L4:
    //   j = j - 1
    // so quads have 5 IRs
    // label[3] = 0
    // label[1] = 0
    // label[5] = 1
    // label[4] = 4
};