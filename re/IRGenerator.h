#pragma once
#include <vector>

#include "AstVisitor.h"
#include "IR.h"

using std::vector;

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

    int defineLabel() {
        labels.push_back(0);

        // L0 is a special label
        // label starts from L1
        return labels.size();
    }

    void markLabel(int label) {
        labels[label] = quads.size();
    }

    ExprNode visitExpr(ExprNode expr) override;

private:
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