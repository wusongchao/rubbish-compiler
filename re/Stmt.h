#pragma once
#include "Ast.h"
#include "Expr.h"
//class Stmt;

class Stmt :
	public Ast
{
public:
	Stmt();
	virtual shared_ptr<Ast> accept(AstVisitor& visitor);
	~Stmt();
};

using StmtNode = shared_ptr<Stmt>;

class Assign : public Stmt {
public:
	Assign(shared_ptr<Id> id_, ExprNode expr_);

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
    virtual AstNode accept(AstVisitor& visitor) override;

    BodyNode body;

    Program(BodyNode body);
private:
};

using ProgramNode = shared_ptr<Program>;

using AssignNode = shared_ptr<Assign>;

using IfNode = shared_ptr<If>;

using WhileNode = shared_ptr<While>;

using CallNode = shared_ptr<Call>;

using ReadNode = shared_ptr<Read>;

using WriteNode = shared_ptr<Write>;

