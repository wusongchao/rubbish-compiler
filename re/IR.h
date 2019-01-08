#pragma once
#include <string>
#include "Expr.h"

using std::string;

namespace IR {


enum class Opcode {
    // Binary operator
    Add,
    Sub,
    Mul,
    Div,

    // Unary operator
    Neg,
    Not,
    Assign,

    Load,     // similar to read
    Store,     // similar to write
    Jt,    // jump if true 
    Jf,    // jump if false
    Goto,
    Param,
    Call,
    Label
};

class Var {
public:
    int width;
    int offset = 0;

    Var(int width, int offset = 0);
    virtual string toString();
};

class Id : public Var {
public:
    string name;

    Id(IdNode node);

    virtual string toString() override;
};

class Temp : public Var{
public:
    string name;

    Temp(int number, const Type& type);
    Temp(int number, int width);

    virtual string toString() override;
};

class Constant : public Var {
public:
    Constant(int width);
};

class Integer : public Constant {
public:
    int value;

    Integer(int value);
    virtual string toString() override;
};

class Bool : public Constant {
public:
    bool value;

    Bool(bool value);
    virtual string toString() override;
};

string opcodeToString(Opcode opcode);

using VarNode = shared_ptr<Var>;

struct Quad {
    Opcode op;

    VarNode src1{ nullptr };
    VarNode src2{ nullptr };
    VarNode result{ nullptr };
    
    Quad(Opcode opcode);
    Quad(Opcode opcode, VarNode src);
    Quad(Opcode opcode, VarNode src, VarNode dest);
    Quad(Opcode opcode, VarNode src1, VarNode src2, VarNode dest);

    // (op, src1, src2, dest)
    string toString() const;
};


}
