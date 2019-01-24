#include <sstream>

#include "IR.h"

using std::ostringstream;
using std::to_string;


namespace IR {

Var::Var(VarTag tag, int width, int offset):
    tag(tag), width(width), offset(offset)
{
}

string Var::toString()
{
    return string();
}

Id::Id(AST::IdNode node)
    :Var(VarTag::Id, node->type.width, node->offset), name(node->token.value)
{
}

Id::Id(AST::Id* node)
    : Var(VarTag::Id, node->type.width, node->offset), name(node->token.value)
{
}

string Id::toString()
{
    return name;
}

Temp::Temp(int number, const Type& type)
    :Var(VarTag::Temp, type.width), name('t' + to_string(number))
{
}

Temp::Temp(int number, int width)
    :Var(VarTag::Temp, width), name('t' + to_string(number))
{
}

string Temp::toString()
{
    return name;
}

Constant::Constant(VarTag tag, int width)
    :Var(tag, width)
{
}

Integer::Integer(int value)
    :Constant(VarTag::Integer, 4), value(value)
{
}

string Integer::toString()
{
    return to_string(value);
}

Bool::Bool(bool value)
    :Constant(VarTag::Bool, 1), value(value)
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
        case Opcode::LT:
            return "<";
        case Opcode::LE:
            return "<=";
        case Opcode::GT:
            return ">";
        case Opcode::GE:
            return ">=";
        case Opcode::EQ:
            return "==";
        case Opcode::NE:
            return "!=";
        case Opcode::Odd:
            return "odd";
        case Opcode::Not:
            return "!";
        case Opcode::Assign:
            return "=";
        case Opcode::Load:
            return "load";
        case Opcode::Store:
            return "store";
        case Opcode::Read:
            return "read";
        case Opcode::Write:
            return "write";
        case Opcode::Jlt:
            return "j<";
        case Opcode::Jle:
            return "j<=";
        case Opcode::Jgt:
            return "j>";
        case Opcode::Jge:
            return "j>=";
        case Opcode::Jeq:
            return "j==";
        case Opcode::Jne:
            return "j!=";
        case Opcode::Jt:
            return "jtrue";
        case Opcode::Jf:
            return "jfalse";
        case Opcode::Jodd:
            return "Jodd";
        case Opcode::Jnodd:
            return "Jnodd";
        case Opcode::Jmp:
            return "jmp";
        case Opcode::Param:
            return "param";
        case Opcode::Call:
            return "call";
        case Opcode::Label:
            return "label";
    }
    return "";
}

Quad::Quad(Opcode opcode)
    :op(opcode)
{
}

Quad::Quad(Opcode opcode, VarNode src)
    :op(opcode), src1(std::move(src))
{
}

Quad::Quad(Opcode opcode, VarNode src, VarNode dest)
    :op(opcode), src1(std::move(src)), result(std::move(dest))
{
}

Quad::Quad(Opcode opcode, VarNode src1, VarNode src2, VarNode dest)
    :op(opcode), src1(std::move(src1)), src2(std::move(src2)), result(std::move(dest))
{
}

string Quad::toString() const
{
    ostringstream stream;
    if (op == Opcode::Label) {
        stream << opcodeToString(op) << src1->toString();

        // function label
        if (src2 != nullptr) {
            stream << ' ' << src2->toString();
        }

        stream << ':';
    } else {
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
    }

    return stream.str();
}

}

