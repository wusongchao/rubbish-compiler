#pragma once
#include "IR.h"
namespace ObjectCode {

using IR::VarNode;

class Code {
public:
	virtual string toString();
};

class Read : public Code {
public:
	int depth;
	int offset;
	Read(int depth, int offset)
		:depth(depth), offset(offset)
	{
	}
	string toString() override;
};

class Load : public Code {
public:
	int depth;
	int offset;
	Load(int depth, int offset)
		:depth(depth), offset(offset)
	{
	}
	string toString() override;
};

class Lit : public Code {
public:
	string constant;
	Lit(const string &constant)
		:constant(constant)
	{
	}
	Lit(string&& constant)
		:constant(std::move(constant))
	{
	}
	string toString() override;
};

class Store : public Code {
public:
	int depth;
	int offset;
	Store(int depth, int offset)
		:depth(depth), offset(offset)
	{
	}
	string toString() override;
};

class BinaryOp : public Code {
public:
	string opStr;
	BinaryOp(const string& opType)
		:opStr(opType)
	{
	}
	BinaryOp(string&& opType)
		:opStr(std::move(opType))
	{
	}
	string toString() override;
};

class Write :public Code {
public:
	string toString() override;
};

class Int : public Code {
public:
	int value;
	Int(int value)
		:value(value)
	{
	}
	string toString() override;
};

class Target : public Code {
public:
    int target;

    Target(int target)
        :target(target)
    {
    }
};

class Jmp : public Target {
public:
	string op;

	Jmp(int target, const string& op)
		:Target(target), op(op)
	{
	}
	Jmp(int target, string&& op)
		:Target(target), op(std::move(op))
	{
	}
	string toString() override;
};

class Call : public Target {
public:
	Call(int target)
		:Target(target)
	{
	}
	string toString() override;
};

}