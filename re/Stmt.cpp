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
	:cond(std::move(l_)), stmt(std::move(s_))
{
}

AstNode While::accept(AstVisitor & visitor)
{
    return visitor.visitWhile(static_pointer_cast<While>(shared_from_this()));
}

If::If(ExprNode l, StmtNode s1_, StmtNode s2_)
	: cond(std::move(l)), trueStmt(std::move(s1_)), falseStmt(std::move(s2_))
{
}

If::If(ExprNode l, StmtNode s1_)
	: cond(std::move(l)), trueStmt(std::move(s1_)), falseStmt(nullptr)
{
}

AstNode If::accept(AstVisitor & visitor)
{
    return visitor.visitIf(static_pointer_cast<If>(shared_from_this()));
}

Assign::Assign(IdNode id_, ExprNode expr_)
	: id(std::move(id_)), expr(std::move(expr_))
{
}

Call::Call(IdNode id_, const vector<ExprNode>& param_)
	: id(std::move(id_)), param(std::move(param_))
{
}

Call::Call(IdNode id_, vector<ExprNode>&& params_)
    :id(std::move(id_)), param(std::move(params_))
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
    return visitor.visitBlock(static_pointer_cast<Block>(shared_from_this()));
}

Block::Block(vector<ProcNode> p, BodyNode b, shared_ptr<Env> t, shared_ptr<ConstEnv> cT)
	:procs(std::move(p)), body(std::move(b)),top(std::move(t)),constTop(std::move(cT))
{
}

Block::Block(vector<ProcNode> procs, BodyNode body)
	:procs(std::move(procs)), body(std::move(body))
{
}

AstNode Proc::accept(AstVisitor & visitor)
{
    return visitor.visitProcedure(static_pointer_cast<Proc>(shared_from_this()));
}

Proc::Proc(IdNode id_, BlockNode block_, shared_ptr<FuncEnv> preFunc)
	:id(std::move(id_)), block(std::move(block_)),preFunc(std::move(preFunc))
{
}

Proc::Proc(IdNode id_, BlockNode block_)
	:id(std::move(id_)), block(std::move(block_))
{
}

void Proc::setPreFunc(shared_ptr<FuncEnv> preFunc)
{
	this->preFunc = std::move(preFunc);
}
