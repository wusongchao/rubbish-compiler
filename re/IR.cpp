#include "IR.h"

string opcodeToString(Opcode opcode)
{
    switch (opcode) {
        case Opcode::Add:
            return "+";
        case Opcode::Sub:
            return "-";
        case Opcode::Mul:
            return "*";
        case Opcode::Div:
            return "/";
        case Opcode::Load:
            return "";
        case Opcode::Store:
            return "";
        case Opcode::Jt:
            return "";
        case Opcode::Jf:
            return "";
        case Opcode::Goto:
            return "goto";
        case Opcode::Param:
            return "param";
        case Opcode::Call:
            return "call";
        case Opcode::Label:
            return "label";
    }
}

string Quad::toString()
{
    switch (op) {
        case Opcode::Add:
        case Opcode::Sub:
        case Opcode::Mul:
        case Opcode::Div:
            return dest.variable->toString() + '=' + 
                src1.variable->toString() + opcodeToString(op) + src2.variable->toString();
    }
}
