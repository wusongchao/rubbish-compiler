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
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    Odd,

    // Unary operator
    Neg,
    Not,
    Assign,

    Load,     
    Store,    
    Read,
    Write,
    Jlt,
    Jle,
    Jgt,
    Jge,
    Jeq,
    Jne,
    Jt,    // jump if true 
    Jf,    // jump if false
    Jodd,    // jump if odd src1
    Jnodd,    // jump if not odd
    Jmp,
    Param,
    Call,
    Label,
    Unknown
};

enum class VarTag {
    Id,
    Temp,
    Constant,
    Integer,
    Bool,
};

class Var {
public:
    VarTag tag;
    int width;
    int offset = 0;

    Var(VarTag tag, int width, int offset = 0);
    virtual string toString();
};

class Id : public Var {
public:
    string name;

    Id(AST::IdNode node);
    Id(AST::Id* id);

    string toString() override;
};

class Func : public Id {
public:
    Func();
};

class Temp : public Var{
public:
    string name;

    Temp(int number, const Type& type);
    Temp(int number, int width);

    string toString() override;
};

class Constant : public Var {
public:
    Constant(VarTag tag, int width);
};

class Integer : public Constant {
public:
    int value;

    Integer(int value);
    string toString() override;
};

class Bool : public Constant {
public:
    bool value;

    Bool(bool value);
    string toString() override;
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

    shared_ptr<Quad> next;
    // (op, src1, src2, dest)
    string toString() const;
};

using QuadPtr = shared_ptr<Quad>;

}
