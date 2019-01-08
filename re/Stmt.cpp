#include "Stmt.h"
#include "AstVisitor.h"

shared_ptr<Ast> Assign::accept(AstVisitor & visitor)
{
	return visitor.visitAssign(static_pointer_cast<Assign>(shared_from_this()));
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

While::While(ExprNode l_, StmtNode s_)
	:cond(l_), stmt(s_)
{
}

If::If(ExprNode l, StmtNode s1_, StmtNode s2_)
	: cond(l), trueStmt(s1_), falseStmt(s2_)
{
}

If::If(ExprNode l, StmtNode s1_)
	: cond(l), trueStmt(s1_), falseStmt(nullptr)
{
}

Assign::Assign(shared_ptr<Id> id_, ExprNode expr_)
	: id(id_), expr(expr_)
{
}

Call::Call(IdNode id_, const vector<ExprNode>& param_)
	: id(id_), param(param_)
{
}

Read::Read(const vector<IdNode>& data)
	: datas(data)
{
}

Read::Read(vector<IdNode>&& data)
    :datas(std::move(data))
{
}

Write::Write(const vector< ExprNode >& data)
	: datas(data)
{
}

Write::Write(vector<ExprNode>&& data)
    :datas(std::move(data))
{
}

Body::Body(const vector<StmtNode>& stmts_)
	:stmts(stmts_)
{
}

Body::Body(vector<StmtNode>&& stmts)
    :stmts(std::move(stmts))
{
}

AstNode Body::accept(AstVisitor & visitor)
{
    return visitor.visitBody(static_pointer_cast<Body>(shared_from_this()));
}
