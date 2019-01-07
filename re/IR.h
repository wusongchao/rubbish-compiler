#pragma once
#include <string>
#include "Expr.h"

using std::string;

enum class Opcode {
    Add, 
    Sub, 
    Mul, 
    Div, 
    Load,     // similar to read
    Store,     // similar to write
    Jt,    // jump if true 
    Jf,    // jump if false
    Goto, 
    Param, 
    Call, 
    Label
};

struct Var {
    bool isTemp = false;
    shared_ptr<Expr> variable;
};

string toString(Opcode opcode);

// (op, src1, src2, dest)
struct Quad {
    Opcode op;

    Var src1;
    Var src2;
    Var dest;

    string toString();
};