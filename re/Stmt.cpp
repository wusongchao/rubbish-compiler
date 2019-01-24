#include "Stmt.h"
#include "AstVisitor.h"

namespace AST {

Ast* Assign::accept(AstVisitor & visitor)
{
    return visitor.visitAssign(this);
}


Stmt::Stmt()
{
}

Ast* Stmt::accept(AstVisitor & visitor)
{
    //throw string("Unexpected call accept() of Stmt!");
    return nullptr;
}

While::While(ExprNode l_, StmtNode s_)
    :cond(std::move(l_)), stmt(std::move(s_))
{
}

Ast* While::accept(AstVisitor & visitor)
{
    return visitor.visitWhile(this);
}

If::If(ExprNode l, StmtNode s1_, StmtNode s2_)
    : cond(std::move(l)), trueStmt(std::move(s1_)), falseStmt(std::move(s2_))
{
}

If::If(ExprNode l, StmtNode s1_)
    : cond(std::move(l)), trueStmt(std::move(s1_)), falseStmt(nullptr)
{
}

Ast* If::accept(AstVisitor & visitor)
{
    return visitor.visitIf(this);
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
    : id(std::move(id_)), param(std::move(params_))
{
}

Ast* Call::accept(AstVisitor & visitor)
{
    return visitor.visitCall(this);
}

Read::Read(const vector<IdNode>& data)
    : datas(data)
{
}

Read::Read(vector<IdNode>&& data)
    : datas(std::move(data))
{
}

Ast* Read::accept(AstVisitor & visitor)
{
    return visitor.visitRead(this);
}

Write::Write(const vector< ExprNode >& data)
    : datas(data)
{
}

Write::Write(vector<ExprNode>&& data)
    : datas(std::move(data))
{
}

Ast* Write::accept(AstVisitor & visitor)
{
    return visitor.visitWrite(this);
}

Body::Body(const vector<StmtNode>& stmts_)
    :stmts(stmts_)
{
}

Body::Body(vector<StmtNode>&& stmts)
    : stmts(std::move(stmts))
{
}

Ast* Body::accept(AstVisitor & visitor)
{
    return visitor.visitBody(this);
}

Ast* Program::accept(AstVisitor & visitor)
{
    return visitor.visitProgram(this);
}

Program::Program(CodeToken id, BlockNode block)
    :id(id), block(std::move(block))
{
}

Ast* Block::accept(AstVisitor & visitor)
{
    return visitor.visitBlock(this);
}

Block::Block(vector<ProcNode> p, BodyNode b, shared_ptr<Env> t, shared_ptr<ConstEnv> cT)
    :procs(std::move(p)), body(std::move(b)), top(std::move(t)), constTop(std::move(cT))
{
}

Block::Block(vector<ProcNode> procs, BodyNode body)
    : procs(std::move(procs)), body(std::move(body))
{
}

Ast* Proc::accept(AstVisitor & visitor)
{
    return visitor.visitProcedure(this);
}

Proc::Proc(IdNode id_, BlockNode block_, shared_ptr<FuncEnv> preFunc)
    :id(std::move(id_)), block(std::move(block_)), preFunc(std::move(preFunc))
{
}

Proc::Proc(IdNode id_, BlockNode block_)
    : id(std::move(id_)), block(std::move(block_))
{
}

void Proc::setPreFunc(shared_ptr<FuncEnv> preFunc)
{
    this->preFunc = std::move(preFunc);
}

}