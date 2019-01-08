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
	If(shared_ptr<Expr> l, StmtNode s1_, StmtNode s2_);
	If(shared_ptr<Expr> l, StmtNode s1_);
	shared_ptr<Expr> lexp;
	StmtNode s1;
	StmtNode s2;
};

class While : public Stmt {
public:
	While(shared_ptr<Expr>, StmtNode s1);
	shared_ptr<Expr> lexp;
	StmtNode s;
};

class Call : public Stmt {
public:
	Call(shared_ptr<Id> id_, std::vector<ExprNode> param_);
	shared_ptr<Id> id;
	std::vector<ExprNode> param;
};

class Read : public Stmt {
public:
	Read(std::vector< shared_ptr<Id> > data);
	std::vector< shared_ptr<Id> > datas;
};

class Write : public Stmt {
public:
	Write(std::vector< ExprNode > data);
	std::vector< ExprNode > datas;
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

