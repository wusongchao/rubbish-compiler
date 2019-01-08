#include <sstream>

#include "IR.h"

using std::ostringstream;
using std::to_string;


namespace IR {

Var::Var(int width, int offset):
    width(width), offset(offset)
{
}

string Var::toString()
{
    return string();
}

Id::Id(IdNode node)
    :Var(node->type.width, node->offset), name(node->token.value)
{
}

string Id::toString()
{
    return name;
}

Temp::Temp(int number, const Type& type)
    :Var(type.width), name('t' + to_string(number))
{
}

Temp::Temp(int number, int width)
    :Var(width), name('t' + to_string(number))
{
}

string Temp::toString()
{
    return name;
}

Constant::Constant(int width)
    :Var(width)
{
}

Integer::Integer(int value)
    :Constant(4), value(value)
{
}

string Integer::toString()
{
    return to_string(value);
}

Bool::Bool(bool value)
    :Constant(1), value(value)
{
}

string Bool::toString()
{
    if (value == true) {
        return "true";
    } else {
        return "false";
    }
}

string opcodeToString(Opcode opcode)
{
    switch (opcode) {
        case Opcode::Add:
            return "+";
        case Opcode::Sub:
        case Opcode::Neg:
            return "-";
        case Opcode::Mul:
            return "*";
        case Opcode::Div:
            return "/";
        case Opcode::Not:
            return "!";
        case Opcode::Assign:
            return "=";
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

Quad::Quad(Opcode opcode)
    :op(opcode)
{
}

Quad::Quad(Opcode opcode, VarNode src)
    :op(opcode), src1(src)
{
}

Quad::Quad(Opcode opcode, VarNode src, VarNode dest)
    :op(opcode), src1(src), result(dest)
{
}

Quad::Quad(Opcode opcode, VarNode src1, VarNode src2, VarNode dest)
    :op(opcode), src1(src1), src2(src2), result(dest)
{
}

string Quad::toString()
{
    ostringstream stream;
    stream << '(' << opcodeToString(op) << ", ";
    if (src1 == nullptr) {
        stream << '-';
    } else {
        stream << src1->toString();
    }
    stream << ", ";
    if (src2 == nullptr) {
        stream << '-';
    } else {
        stream << src2->toString();
    }
    stream << ", ";
    if (result == nullptr) {
        stream << '-';
    } else {
        stream << result->toString();
    }
    stream << ')';

    return stream.str();
}

}

