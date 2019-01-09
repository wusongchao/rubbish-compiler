#pragma once
#include <vector>
#include <stack>

#include "AstVisitor.h"
#include "IR.h"

using std::vector;
using std::make_shared;
using std::stack;
using std::ostream;

using IR::Quad;
using IR::QuadPtr;
using IR::Var;
using IR::VarNode;
using IR::Opcode;

class IRGenerator : public AstVisitor{
public:

    const vector<QuadPtr>& getQuads() const
    {
        return quads;
    }

    const vector<QuadPtr> getLabels() const
    {
        return labels;
    }

    void output(ostream& output) const
    {
        if (quads.empty()) {
            return;
        }

        auto quad = quads[0];
        while (quad != nullptr) {
            output << quad->toString() << std::endl;
            quad = quad->next;
        }
    }

    AstNode visitProgram(shared_ptr<Program> program) override;

    AstNode visitBody(BodyNode body) override;

    AstNode visitAssign(AssignNode assign) override;

    AstNode visitExpr(ExprNode expr) override;

    AstNode visitIf(IfNode ifNode) override;

    AstNode visitWhile(WhileNode whileNode) override;

    AstNode visitCall(CallNode call) override;

    AstNode visitRead(ReadNode read) override;

    AstNode visitWrite(WriteNode write) override;

    AstNode visitLogical(LogicalNode logical) override;

    AstNode visitOr(OrNode orNode) override;

    AstNode visitAnd(AndNode andNode) override;

    AstNode visitNot(NotNode notNode) override;

    AstNode visitOdd(OddNode oddNode) override;

    AstNode visitRel(RelNode rel) override;

    AstNode visitArith(ArithNode arith) override;

    AstNode visitOp(OpNode op) override;

    AstNode visitUnary(UnaryNode unary) override;

    AstNode visitConstant(ConstantNode constant) override;

    AstNode visitId(IdNode id) override;

private:
    //TempNode emitBinary(const CodeToken& opToken, const Type& resType, const ExprNode& lhs, const ExprNode& rhs);

    shared_ptr<IR::Temp> createTemp(const Type& type) {
        return make_shared<IR::Temp>(tempIndex++, type);
    }

    shared_ptr<IR::Temp> createTemp(int width) {
        return make_shared<IR::Temp>(tempIndex++, width);
    }

    void appendQuad(QuadPtr quad) {
        auto quadsSize = quads.size();
        if (quadsSize != 0) {
            quads[quadsSize - 1]->next = quad;
        }
        quads.push_back(quad);
    }

    void emitConditionJmp(Opcode op, VarNode src1, VarNode src2, VarNode dest) {
        appendQuad(make_shared<Quad>(op, src1, src2, dest));
        //quads.emplace_back(op, src1, src2, dest);
    }

    Opcode getOpcode(CodeTokenType opTokenType) {
        switch (opTokenType) {
            case CodeTokenType::LT:
                return Opcode::LT;
            case CodeTokenType::LE:
                return Opcode::LE;
            case CodeTokenType::GT:
                return Opcode::GT;
            case CodeTokenType::GE:
                return Opcode::GE;
            case CodeTokenType::EQ:
                return Opcode::EQ;
            case CodeTokenType::NE:
                return Opcode::NE;
            case CodeTokenType::Odd:
                return Opcode::Odd;
        }
        return Opcode::Unknown;
    }

    Opcode getJmpOpcode(CodeTokenType opTokenType) {
        switch (opTokenType) {
            case CodeTokenType::LT:
                return Opcode::Jlt;
            case CodeTokenType::LE:
                return Opcode::Jle;
            case CodeTokenType::GT:
                return Opcode::Jgt;
            case CodeTokenType::GE:
                return Opcode::Jge;
            case CodeTokenType::EQ:
                return Opcode::Jeq;
            case CodeTokenType::NE:
                return Opcode::Jne;
            case CodeTokenType::Odd:
                return Opcode::Jodd;
        }
        return Opcode::Unknown;
    }

    void emitConditionJmp(Opcode op, VarNode src1, VarNode src2, int dest) {
        auto destLabel{ make_shared<IR::Integer>(dest) };
        emitConditionJmp(op, src1, src2, destLabel);
    }

    void emitJmp(VarNode dest) {
        auto jmp{ make_shared<Quad>(Opcode::Jmp) };
        jmp->result = dest;
        appendQuad(jmp);
        //quads.push_back(jmp);
    }

    void emitJmp(int label) {
        auto dest{ make_shared<IR::Integer>(label) };
        emitJmp(dest);
    }

    void emitOddJmp(Opcode op, VarNode src, VarNode dest) {
        appendQuad(make_shared<Quad>(op, src, dest));
    }

    void emitOddJmp(Opcode op, VarNode src, int label) {
        auto dest{ make_shared<IR::Integer>(label) };
        emitOddJmp(op, src, dest);
    }

    void emitArith(Opcode op, VarNode src1, VarNode src2, VarNode dest) {
        appendQuad(make_shared<Quad>(op, src1, src2, dest));
        //quads.emplace_back(op, src1, src2, dest);
    }

    // will not push logical ir to quads
    //void emitLogical(Opcode op, VarNode src1, VarNode src2) {
    //    auto logical{ make_shared<Quad>(op) };
    //    logical->src1 = src1;
    //    logical->src2 = src2;
    //    logicalStack.push(logical);
    //}

    void emitUnary(Opcode op, VarNode src, VarNode dest) {
        appendQuad(make_shared<Quad>(op, src, dest));
    }

    void emitAssign(VarNode src, VarNode dest) {
        appendQuad(make_shared<Quad>(Opcode::Assign, src, dest));
    }

    void emitParam(VarNode param) {
        appendQuad(make_shared<Quad>(Opcode::Param, param));
    }

    void emitRead(VarNode id) {
        appendQuad(make_shared<Quad>(Opcode::Load, id));
    }

    void emitWrite(VarNode exp) {
        appendQuad(make_shared<Quad>(Opcode::Write, exp));
    }

    void emitCall(VarNode funcName) {
        auto call{ make_shared<Quad>(Opcode::Call) };
        call->result = funcName;
        appendQuad(call);
    }

    int emitLabel() {
        int number = labels.size();
        auto label{ make_shared<Quad>(Opcode::Label, make_shared<IR::Integer>(number)) };
        labels.push_back(label);
        // for label quad, the src1 is the label number, result is the target number
        //quads.emplace_back(Opcode::Label, make_shared<IR::Integer>(number));
        //appendQuad(label);
        return number;
    }

    // markLabel: the label jump to where
    // insert to the proper position when the position can be determined
    
    void markLabel(int label) {
        // for label quad, the src1 is the label sequence number, result is the target number
        labels[label]->result = make_shared<IR::Integer>(quads.size());
        appendQuad(labels[label]);
        // skip one quad: the label quad
    }

    Opcode getOppositeConditonJmpOpcode(Opcode op) {
        switch (op)
        {
            case Opcode::LT:
            case Opcode::Jlt:
                return Opcode::Jge;
            case Opcode::LE:
            case Opcode::Jle:
                return Opcode::Jgt;
            case Opcode::GT:
            case Opcode::Jgt:
                return Opcode::Jle;
            case Opcode::GE:
            case Opcode::Jge:
                return Opcode::Jlt;
            case Opcode::EQ:
            case Opcode::Jeq:
                return Opcode::Jne;
            case Opcode::NE:
            case Opcode::Jne:
                return Opcode::Jeq;
            case Opcode::Odd:
            case Opcode::Jodd:
                return Opcode::Jnodd;
            case Opcode::Jnodd:
                return Opcode::Jodd;
        }
        return Opcode::Jmp;
    }

    int tempIndex = 1;
    vector<QuadPtr> quads;
    vector<QuadPtr> labels;
    stack<VarNode> operateStack;
    // see the comment in visitRel, visitIf

    // for convenience, this is used to pass property true, false
    // to logical expression's translation process
    // can use QuadPtr stack, but stack<int> need not to do the conversion
    // for more information, see the comment in the head of file "IRGenerator.cpp"
    stack<int> labelStack;
    
    // a special label, see the "dragon book" p261
    static constexpr int Fall = -1;

    // if a then b
    // 0: if not a jmp L0
    // 1: b
    // L0: 2:
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