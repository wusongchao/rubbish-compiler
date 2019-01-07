#pragma once
#include <string>
#include "Expr.h"

using std::string;

enum class Opcode {
    // Binary operator
    Add, 
    Sub, 
    Mul, 
    Div, 
    
    // Unary operator
    Neg,
    Not,

    Load,     // similar to read
    Store,     // similar to write
    Jt,    // jump if true 
    Jf,    // jump if false
    Goto, 
    Param, 
    Call, 
    Label
};

enum class VarTag {
    Id, Constant, Temp, Empty
};

struct Var {
    VarTag tag = VarTag::Empty;
    shared_ptr<Expr> variable = nullptr;
};

string opcodeToString(Opcode opcode);

// (op, src1, src2, dest)
struct Quad {
    Opcode op;

    Var src1;
    Var src2;
    Var dest;

    string toString();
};