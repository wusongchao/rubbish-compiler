#pragma once
#include <vector>

#include "AstVisitor.h"
#include "IR.h"

using std::vector;
using std::make_shared;

class IRGenerator : public AstVisitor {
public:
    const vector<Quad>& getQuads() const
    {
        return quads;
    }

    const vector<int> getLabels() const
    {
        return labels;
    }

    ExprNode visitExpr(ExprNode expr) override;

    // a = a + 1 + 2 + 3
    // => t1 = 2 + 3
    // => t2 = 1 + t1
    //    a = a + t2

    // a = a + 1
    // => a = a + 1
    OpNode visitOp();

    ExprNode visitArith(ArithNode node);

    //AssignNode visitAssign(AssignNode node);
private:
    void emitGoto(int label) {
        quads.emplace_back();
    }

    //TempNode emitBinary(const CodeToken& opToken, const Type& resType, const ExprNode& lhs, const ExprNode& rhs);

    TempNode createTemp(const Type& type) {
        return make_shared<Temp>(tempIndex++, type);
    }

    void emitConditionJmp() {

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