#pragma once
#include "Ast.h"
#include "Expr.h"
class Proc;
class Block;
class Env;
class FuncEnv;
class ConstEnv;
using ProcNode = shared_ptr<Proc>;
using BlockNode = shared_ptr < Block>;


class Stmt :
	public Ast
{
public:
	Stmt();
	virtual shared_ptr<Ast> accept(AstVisitor& visitor);
	~Stmt();
};

using StmtNode = shared_ptr<Stmt>;

// why take a value shared_ptr as input?
// the class ctor will copy it anyway
class Assign : public Stmt {
public:
	Assign(IdNode id_, ExprNode expr_);

	shared_ptr<Ast> accept(AstVisitor& visitor) override;

	shared_ptr<Id> id;
	ExprNode expr;
};

class If : public Stmt {
public:
	If(ExprNode l, StmtNode s1_, StmtNode s2_);
	If(ExprNode l, StmtNode s1_);

    AstNode accept(AstVisitor& visitor) override;
    ExprNode cond;
	StmtNode trueStmt;
	StmtNode falseStmt;
};

class While : public Stmt {
public:
	While(ExprNode, StmtNode s1);

    AstNode accept(AstVisitor& visitor) override;

    ExprNode cond;
	StmtNode stmt;
};

class Call : public Stmt {
public:
	Call(IdNode id_, const vector<ExprNode>& param_);
    Call(IdNode id_, vector<ExprNode>&& params_);
    
    AstNode accept(AstVisitor& visitor) override;

	IdNode id;
	vector<ExprNode> param;
};

class Read : public Stmt {
public:
	Read(const vector<IdNode>& data);
    Read(vector<IdNode>&& data);

    AstNode accept(AstVisitor& visitor) override;

	vector<IdNode> datas;
};

class Write : public Stmt {
public:
	Write(const vector<ExprNode>& data);
    Write(vector<ExprNode>&& data);

    AstNode accept(AstVisitor& visitor) override;

	vector<ExprNode> datas;
};

class Body : public Stmt{
public:
	Body(const vector<StmtNode>& stmts);
    Body(vector<StmtNode>&& stmts);
    vector<StmtNode> stmts;

    AstNode accept(AstVisitor& visitor) override;
};

using BodyNode = shared_ptr<Body>;

class Program : public Ast {
public:
    AstNode accept(AstVisitor& visitor) override;
 //   BodyNode body;
	//ProcNode proc;
 //   Program(BodyNode body);
	//Program(BodyNode body, ProcNode proc);

	CodeToken id;
	BlockNode block;
	Program(CodeToken id,BlockNode block);

private:
};

class Block : public Ast {
public:
	AstNode accept(AstVisitor& visitor) override;
	Block(vector<ProcNode> procs, BodyNode b, shared_ptr<Env> t, shared_ptr<ConstEnv> cT);
	Block(vector<ProcNode> procs, BodyNode b);
	vector<ProcNode> procs;	//procedure
	BodyNode body;
	shared_ptr<Env> top;
	shared_ptr<ConstEnv> constTop;
};

class Proc :public Ast {
public:
	AstNode accept(AstVisitor& visitor) override;
	Proc(IdNode id_, BlockNode block_, shared_ptr<FuncEnv> preFunc);
	Proc(IdNode id_, BlockNode block_);
	void setPreFunc(shared_ptr<FuncEnv> preFunc);
	IdNode id;
	BlockNode block;
	shared_ptr<FuncEnv> preFunc;
};

using ProgramNode = shared_ptr<Program>;

using AssignNode = shared_ptr<Assign>;

using IfNode = shared_ptr<If>;

using WhileNode = shared_ptr<While>;

using CallNode = shared_ptr<Call>;

using ReadNode = shared_ptr<Read>;

using WriteNode = shared_ptr<Write>;

