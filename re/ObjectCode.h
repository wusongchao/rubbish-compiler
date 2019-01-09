#pragma once
#include "IR.h"
namespace ObjectCode {

	using IR::VarNode;

	class ObjectCode {
	public:
		virtual string toString();
	};

	class Read : public ObjectCode {
	public:
		int depth;
		int offset;
		Read(int depth, int offset)
			:depth(depth), offset(offset)
		{
		}
		string toString() override;
	};

	class Lod : public ObjectCode {
	public:
		int depth;
		int offset;
		Lod(int depth, int offset)
			:depth(depth), offset(offset)
		{
		}
		string toString() override;
	};

	class Lit : public ObjectCode {
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

	class Store : public ObjectCode {
	public:
		int depth;
		int offset;
		Store(int depth, int offset)
			:depth(depth), offset(offset)
		{
		}
		string toString() override;
	};

	class BinaryOp : public ObjectCode {
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

	class Write :public ObjectCode {
	public:
		string toString() override;
	};

	class Int : public ObjectCode {
	public:
		int value;
		Int(int value)
			:value(value)
		{
		}
		string toString() override;
	};

	class Jmp : public ObjectCode {
	public:
		int target;
		string op;

		Jmp(int target, const string& op)
			:target(target), op(op)
		{
		}
		Jmp(int target, string&& op)
			:target(target), op(std::move(op))
		{
		}
		string toString() override;
	};

	class Call : public ObjectCode {
	public:
		string func;
		Call(const string& func)
			:func(func)
		{
		}
		Call(string&& func)
			:func(std::move(func))
		{
		}
		string toString() override;
	};
}