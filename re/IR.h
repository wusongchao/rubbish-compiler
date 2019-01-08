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

enum class VarTag {
    Id, Constant, Temp, Empty
};

struct Var {
    VarTag tag = VarTag::Empty;
    shared_ptr<Expr> variable = nullptr;

    const static Var Empty;
};

string opcodeToString(Opcode opcode);

// (op, src1, src2, dest)
struct Quad {
    Opcode op;

    Var src1 = Var::Empty;
    Var src2 = Var::Empty;
    Var dest = Var::Empty;

    //Quad(Opcode opcode, const Var& src);
    //Quad(Opcode opcode, const Var& src, const Var& dest);

    string toString();
};