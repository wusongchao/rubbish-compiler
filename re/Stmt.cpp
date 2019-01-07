#include "Stmt.h"
#include "AstVisitor.h"

shared_ptr<Ast> Assign::accept(AstVisitor & visitor)
{
	//visitor.visitAssign(static_pointer_cast<Assign>(enable_shared_from_this()));
	return nullptr;
}


Stmt::Stmt()
{
}

shared_ptr<Ast> Stmt::accept(AstVisitor & visitor)
{
	throw string("Unexpected call accept() of Stmt!");
	return shared_ptr<Ast>();
}

Stmt::~Stmt()
{
}

While::While(shared_ptr<Expr> l_, StmtNode s_)
	:lexp(l_), s(s_)
{
}

If::If(shared_ptr<Expr> l, StmtNode s1_, StmtNode s2_)
	: lexp(l), s1(s1_), s2(s2_)
{
}

If::If(shared_ptr<Expr> l, StmtNode s1_)
	: lexp(l), s1(s1)
{
	s2 = nullptr;
}

Assign::Assign(shared_ptr<Id> id_, ExprNode expr_)
	: id(id_), expr(expr_)
{
}

Call::Call(shared_ptr<Id> id_, std::vector<ExprNode> param_)
	: id(id_), param(param_)
{
}

Read::Read(std::vector<shared_ptr<Id>> data)
	: datas(data)
{
}

Write::Write(std::vector< ExprNode > data)
	: datas(data)
{
}

Body::Body(std::vector<StmtNode> stmts_)
	:stmts(stmts_)
{
}
