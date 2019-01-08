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

AstNode While::accept(AstVisitor & visitor)
{
    return visitor.visitWhile(static_pointer_cast<While>(shared_from_this()));
}

If::If(ExprNode l, StmtNode s1_, StmtNode s2_)
	: cond(l), trueStmt(s1_), falseStmt(s2_)
{
}

If::If(ExprNode l, StmtNode s1_)
	: cond(l), trueStmt(s1_), falseStmt(nullptr)
{
}

AstNode If::accept(AstVisitor & visitor)
{
    return visitor.visitIf(static_pointer_cast<If>(shared_from_this()));
}

Assign::Assign(shared_ptr<Id> id_, ExprNode expr_)
	: id(id_), expr(expr_)
{
}

Call::Call(IdNode id_, const vector<ExprNode>& param_)
	: id(id_), param(param_)
{
}

AstNode Call::accept(AstVisitor & visitor)
{
    return visitor.visitCall(static_pointer_cast<Call>(shared_from_this()));
}

Read::Read(const vector<IdNode>& data)
	: datas(data)
{
}

Read::Read(vector<IdNode>&& data)
    :datas(std::move(data))
{
}

AstNode Read::accept(AstVisitor & visitor)
{
    return visitor.visitRead(static_pointer_cast<Read>(shared_from_this()));
}

Write::Write(const vector< ExprNode >& data)
	: datas(data)
{
}

Write::Write(vector<ExprNode>&& data)
    :datas(std::move(data))
{
}

AstNode Write::accept(AstVisitor & visitor)
{
    return visitor.visitWrite(static_pointer_cast<Write>(shared_from_this()));
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

AstNode Block::accept(AstVisitor & visitor)
{
	return AstNode();
}

Block::Block(ProcNode proc_, BodyNode body_)
	:pde(proc_), body(body_)
{
}

AstNode Proc::accept(AstVisitor & visitor)
{
	return AstNode();
}

Proc::Proc(IdNode id_, BlockNode block_, std::vector<ProcNode> procs_)
	:id(id_), block(block_), procs(procs_)
{
}