#pragma once

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

};

struct Quad {
    Opcode op;


};