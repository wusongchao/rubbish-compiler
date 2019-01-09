#pragma once
#include "IR.h"

namespace ObjectCode{

using IR::VarNode;

class ObjectCode {
public:
    virtual string toString();
};

class Read : public ObjectCode {
public:
    int depth;
    int offset;

    string toString() override;
};

}
